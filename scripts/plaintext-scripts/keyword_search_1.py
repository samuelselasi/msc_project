import pandas as pd
import sys

# -----------------------------
# Step 1: Handle CLI argument
# -----------------------------
if len(sys.argv) < 2:
    print("Please provide a keyword to search.")
    print("Usage: python3 keyword_search_1.py confidential")
    sys.exit(1)

search_term = sys.argv[1].lower()
print(f"Searching for keyword: '{search_term}'\n")

# -----------------------------
# Step 2: Load dataset
# -----------------------------
df = pd.read_csv("../../datasets/cleaned/split-1.csv")

# -----------------------------
# Step 3: Define search logic
# -----------------------------
matches = []

for i, row in df.iterrows():
    content = f"{row.get('subject', '')} {row.get('body', '')}".lower()
    if search_term in content:
        matches.append({
            "line": i + 2,  # +2 to reflect actual line in the CSV (header + 1-based)
            "email_id": row.get("email_id", "unknown"),
            "date": row.get("date", "N/A"),
            "subject": row.get("subject", ""),
            "body_snippet": row.get("body", "")
        })

# -----------------------------
# Step 4: Output results
# -----------------------------
if not matches:
    print("No matches found.")
else:
    print(f"{len(matches)} match(es) found:\n")
    for m in matches:
        print(f"Line: {m['line']} | ID: {m['email_id']}")
        print(f"Date: {m['date']}")
        print(f"Subject: {m['subject']}")
        print(f"Body: {m['body_snippet']}\n")
        print("=" * 90 + "\n")
