import pandas as pd
import re

# Load the raw email dataset
df = pd.read_csv("../scripts/datasets/raw/emails.csv")

# Select the first 100 emails
subset = df.head(100).copy()

# Prepare output rows
parsed_rows = []

for _, row in subset.iterrows():
    email_id = row['file']
    message = row['message']

    # Split into header and body
    parts = message.split("\n\n", 1)
    headers = parts[0]
    body = parts[1] if len(parts) > 1 else ""

    # Function to extract a field from headers
    def extract_header(header_text, field):
        match = re.search(rf"^{field}:\s*(.*)", header_text, re.MULTILINE | re.IGNORECASE)
        return match.group(1).strip() if match else ""

    # Extract relevant headers
    from_field = extract_header(headers, "From")
    to_field = extract_header(headers, "To")
    cc_field = extract_header(headers, "Cc")
    date_field = extract_header(headers, "Date")
    subject_field = extract_header(headers, "Subject")

    # Clean body
    def clean(text):
        text = str(text).strip()
        text = re.sub(r"[^\x00-\x7F]+", "", text)  # remove non-ASCII
        return re.sub(r"\s+", " ", text)

    parsed_rows.append({
        "email_id": email_id,
        "from": clean(from_field),
        "to": clean(to_field),
        "cc": clean(cc_field),
        "date": clean(date_field),
        "subject": clean(subject_field),
        "body": clean(body)
    })

# Create DataFrame and save
output_df = pd.DataFrame(parsed_rows)
output_df.to_csv("../datasets/cleaned/split-1.csv", index=False)

print("Saved 'split-1.csv' with 100 parsed emails and full headers.")
print(output_df.head(3))
