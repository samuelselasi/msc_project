import pandas as pd

# Load the original split dataset
df = pd.read_csv("split-1.csv")

# Clean both subject and body (remove non-ASCII chars)
df["subject"] = df["subject"].apply(lambda x: x.encode("ascii", "ignore").decode() if isinstance(x, str) else "")
df["body"] = df["body"].apply(lambda x: x.encode("ascii", "ignore").decode() if isinstance(x, str) else "")

# Combine both into one 'message' column
df["message"] = df["subject"] + " " + df["body"]

# Save to a cleaned version
df.to_csv("cleaned_split-1.csv", index=False)
print("✅ Cleaned and saved as cleaned_split-1.csv")
