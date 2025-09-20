#!/usr/bin/python3

from fastapi import APIRouter, Depends, HTTPException
from sqlalchemy.orm import Session
from app.oauth2 import oauth2_scheme
from app.routers.user.crud import read_user_by_id
from app import utils
from app.config import settings
from . import crud, models, schemas
from .database import SessionLocal, engine
import subprocess
from typing import List
import json
from app.routers.user.models import User
from app.routers.search.models import SearchLog

# models.Base.metadata.create_all(bind=engine)

router = APIRouter()

# Dependency to get DB session
def get_db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()

def run_cpp_search(keyword: str) -> str:
    binary_path = settings.SEARCH_EXECUTABLE_PATH
    working_dir = "/opt/msc/heksm/scripts/HE-scripts"

    try:
        result = subprocess.run(
            [binary_path],
            input=f"{keyword}\n",
            capture_output=True,
            text=True,
            check=True,
            cwd=working_dir
        )
    except FileNotFoundError:
        raise RuntimeError("Search binary not found.")
    except subprocess.CalledProcessError as e:
        raise RuntimeError(f"Search process failed:\n{e.stderr}")

    return result.stdout.strip()


@router.get("/logs", response_model=List[schemas.SearchLogOut])
def get_logs(db: Session = Depends(get_db)):
    """Endpoint to return all search logs."""
    return crud.get_all_search_logs(db)

@router.get("/logs/user/{user_id}", response_model=List[schemas.SearchLogOut])
def get_logs_by_user(user_id: int, db: Session = Depends(get_db)):
    """Endpoint to return all search logs for a given user."""
    return crud.get_search_logs_by_user(db, user_id)


@router.post("/search", response_model=schemas.SearchOutput)
async def search_keyword(payload: schemas.SearchRequest,
                         token: str = Depends(oauth2_scheme),
                         db: Session = Depends(get_db)):
    token_data = utils.decode_token(data=token)
    user_id = token_data["id"]
    user = await read_user_by_id(user_id, db)

    output = run_cpp_search(payload.keyword)
    cleaned_output = output.replace("Enter keyword to search: ", "")

    crud.log_search(
        db,
        user.id,
        payload.keyword,
        match_count=output.count("Match in file:"),  # crude match count
        decrypted=(user.user_type_id in [1, 2])
    )

    return {"output": cleaned_output}
