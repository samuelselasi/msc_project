# MSc Project: Securing Third-Party Data In Forensic Investigations Using Homomorphic Encryption

**Author:** Samuel Selasi

**University:** University of Ghana

**Thesis Phase:** Implementation Phase 1

**Focus:** Secure keyword-based forensic investigation using Homomorphic Encryption (HE) and comparison with traditional plaintext methods.

---

## Project Description

This repository presents the first phase of the implementation for my Master's thesis titled:

> **"Securing Third-Party Data in Forensic Investigations Using Homomorphic Encryption"**

The goal is to perform forensic keyword searches on sensitive email datasets while preserving privacy using homomorphic encryption (Microsoft SEAL). For comparison, the traditional plaintext approach is also implemented side-by-side. The project simulates forensic scenarios where investigators need to identify relevant content without revealing the full contents of data.

---

## Folder Structure

msc_project/
│
├── datasets/
│   ├── original/                # Raw Enron dataset (emails.csv, full)
│   ├── cleaned/                 # Cleaned and preprocessed CSVs
│   │   ├── split-1.csv          # Selected subset (e.g., 100 emails)
│   │   └── cleaned_split-1.csv  # Sanitized for tokenization
│   └── encrypted/               # Encrypted emails + keys
│       ├── public.key
│       ├── secret.key
│       └── tokenized_emails.json (ignored)
│
├── scripts/
│   ├── plaintext/               # Plaintext search scripts (Python)
│   │   └── search_plaintext.py
│   └── HE-scripts/              # Homomorphic Encryption scripts (C++)
│       ├── encrypt_split_1.cpp
│       ├── search_encrypted.cpp
│       ├── seal/                # SEAL headers
│       ├── libseal.a            # Compiled SEAL static library
│       └── build/               # Compiled binaries and keys
│
├── clean.py                     # Dataset cleaner (removes special chars, encodes)
├── LICENSE                      # MIT License
└── README.md                    # Project description and usage guide


---

## Requirements

### Python (Plaintext Search & Preprocessing)

- Python 3.10+
- pandas
- tqdm

Install dependencies:

```
pip install pandas tqdm
```

- C++ (HE-Based Search)
- Ubuntu 20.04+
- Microsoft SEAL (v4.1 or above)
- `libnlohmann-json-dev` (`sudo apt install nlohmann-json3-dev`)
- g++ with C++17 support
- CMake (optional for build automation)

## Phase 1: Implementation Overview
This phase covers:

1. **Plaintext keyword search** using Python over the Enron email corpus
2. **Homomorphic encryption (BFV scheme)** of email body content using Microsoft SEAL
3. **Encrypted keyword search** using C++ without decrypting the dataset
4. Comparison and evaluation (basic metrics like match count and privacy assurance)


## Dataset

A subset of the **Enron Email Dataset** in CSV form was used.

* **Source**: [Enron Email Dataset](https://www.cs.cmu.edu/~enron/)
* **Subset Used**: 100 emails (manually selected and split)


# Instructions to Replicate Experiments

## 1. Prepare Dataset

```
cd datasets/cleaned
python3 ../../clean.py
```

This will:

* Remove invalid characters

* Lowercase and sanitize text

* Output: `cleaned_split-1.csv`


## 2. Plaintext Keyword Search

```
cd scripts/plaintext
python3 search_plaintext.py brenda

```

* You can change `brenda` to any keyword.

* The script searches across both subject and body fields.

* Output includes matched email IDs and matching lines.


## 3. Compile and Run HE-Based Search
### 3.1 Build Encrypted Dataset

```
cd scripts/HE-scripts
g++ -std=c++17 encrypt_split_1.cpp libseal.a -Iseal -I/usr/include/nlohmann -o encrypt_split_1
./encrypt_split_1
```

This will:

* Tokenize each email

* Encrypt every word using SEAL’s BFV scheme

* Save tokens in base64 form to `tokenized_emails.json`

* Save public/secret keys in `build/`

### 3.2 Run Keyword Search Over Encrypted Emails

```
g++ -std=c++17 search_encrypted.cpp libseal.a -Iseal -I/usr/include/nlohmann -o search_encrypted
./search_encrypted

```

You'll be prompted:

```
🔑 Enter keyword to search: shoot

```

If the keyword matches any encrypted tokens, the script will:

* Decrypt the match result

* Show the full original message

* Print the matching email ID


#### Output Example

```
🔑 Enter keyword to search: shoot
✅ Match in file: allen-p/_sent_mail/1001.
📝 Message: Let's shoot for Tuesday at 11:45.,Re: Hello Let's shoot for Tuesday at 11:45.


🔍 Done. Total matches: 1
```

## Notes
* You can increase the dataset size by modifying the loop in `encrypt_split_1.cpp`

```
if (++email_count > 100) break;
```

* Current implementation only uses **keyword equality matching**. Advanced features like fuzzy search or multi-keyword logic will be explored in future phases.

# Progress

This repository implements **Phase 1** of the methodology described in Chapter 3 of the thesis:

* Establishing a parallel forensic framework

* Implementing searchable encryption using HE

* Building comparison with traditional plaintext workflows

Subsequent phases will cover performance evaluation, scalability tests, and legal alignment under GDPR.

# Contact

Feel free to fork, test, or reach out for feedback!

## License

This project is licensed under the [MIT License](LICENSE).
