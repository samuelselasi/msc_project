import pandas as pd

# Load cleaned dataset
df = pd.read_csv("cleaned_split-1.csv")

# Save to a text file for Bulk Extractor input
with open("bulk_input.txt", "w", encoding="utf-8") as f:
    for _, row in df.iterrows():
        f.write(f"Email ID: {row['email_id']}\n")
        f.write(f"From: {row['from']}\n")
        f.write(f"To: {row['to']}\n")
        f.write(f"CC: {row['cc']}\n")
        f.write(f"Date: {row['date']}\n")
        f.write(f"Subject: {row['subject']}\n")
        f.write(f"Body: {row['body']}\n")
        f.write("\n" + "="*80 + "\n\n")  # separator between emails

print("bulk_input.txt created successfully.")
