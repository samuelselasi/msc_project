# Homomorphic Encryption Scripts for Secure Keyword Search

This directory contains the C++ implementations for **Phase 2** of the master's thesis project:  
**"Securing Third-Party Data in Forensic Investigations Using Homomorphic Encryption."**

The goal of this phase is to demonstrate how **BFV Homomorphic Encryption** (via [Microsoft SEAL](https://github.com/microsoft/SEAL)) can be applied to enable **encrypted keyword searches** on sensitive datasets, such as the Enron email corpus.

---

## Directory Contents

| File                          | Description |
|-------------------------------|-------------|
| `optimized_encryption_1.cpp`    | Encrypts a preprocessed Enron email dataset using BFV and stores the result as encrypted tokens. |
| `optimized_search_1.cpp`        | Accepts a keyword input, encrypts it, and performs a match on the encrypted dataset using homomorphic operations. |
| `encrypt_split_1.cpp`         | Legacy version of the encryption script for testing or comparison. |
| `search_encrypted.cpp`        | Legacy version of the search script for testing or comparison. |
| `libseal.a`                   | Compiled Microsoft SEAL static library. |
| `CMakeLists.txt`              | Optional CMake build script. |
| `build/`                      | Contains the compiled `public.key`, `secret.key`, and optionally the compiled binaries. |
| `datasets/encrypted/`         | Destination for encrypted output files (e.g., `tokenized_emails.json`). |

---

## Prerequisites

Before compiling or running, make sure you have the following installed:

- A C++17-compliant compiler (`g++`, `clang++`, etc.)
- [Microsoft SEAL library](https://github.com/microsoft/SEAL) compiled into `libseal.a`
- [nlohmann/json](https://github.com/nlohmann/json) installed (`sudo apt install nlohmann-json-dev`)
- A cleaned dataset file at `../../datasets/cleaned/cleaned_split-1.csv`

---

## Compilation Instructions

Use the following commands to compile the scripts:

```
# Compile encryption
g++ -std=c++17 optimized_encryption.cpp libseal.a -Iseal -I/usr/include/nlohmann -o optimized_encryption
```

# Compile search

```
g++ -std=c++17 optimized_search.cpp libseal.a -Iseal -I/usr/include/nlohmann -o optimized_search
```

## Phase 1: Homomorphic Encryption of Emails
### What it does:

- Loads the preprocessed CSV file (cleaned_split-1.csv)
- Cleans, tokenizes, encodes, and encrypts each word
- Stores the encrypted result in tokenized_emails.json (base64 format)
- Saves public.key and secret.key to ./build/

## How to run:

```
./optimized_encryption
```

You will see output like:

```
Encryption time: 11.1583 seconds
Processed 6 emails.
```

## Phase 2: Keyword Search over Encrypted Data
### What it does:

- Prompts the user for a keyword
- Encrypts the keyword
- Searches for matches within the encrypted email tokens using SEAL's Evaluator
- Decrypts the comparison results to determine match presence
- Displays matching emails and how long the search took

## How to run:

```
./optimized_search
```

Example prompt/output:

```
Enter keyword to search: shoot
Match in file: allen-p/_sent_mail/1001.
Message: Let's shoot for Tuesday at 11:45.,Re: Hello Let's shoot for Tuesday at 11:45.


Done. Total matches: 1
Search Time: 12.934 seconds
```

## Performance Measurement
Both scripts include precise time tracking to measure:

* Encryption time (`optimized_encryption`)
* Search time (`optimized_search`)

This supports benchmarking HE-based search versus plaintext baseline.


## Notes
* Only the first **5 emails** are processed in this test phase (`email_count > 5`) for speed and reproducibility. Increase this value to scale the experiment.

* All tokens are sanitized, limited to printable ASCII, and encoded via SEAL's **BatchEncoder**.


## Improved Optimizations

This directory implements performance enhancements over the baseline homomorphic encryption setup:

* **Indexed Dictionary Encoding**: Avoids re-encrypting duplicate tokens by referencing a single ciphertext per unique word.

* **Base64 Encoding**: Efficient serialization and storage of ciphertexts in JSON format.

* **Single File Format**: Merges the encrypted dictionary and message-token mappings for streamlined loading.

* **Execution Time Metrics**: Measures encryption and search duration to support performance benchmarking.

These optimizations significantly reduce memory usage, processing time, and redundancy — making homomorphic keyword search more feasible at scale.


## References

* [Microsoft SEAL](https://github.com/microsoft/SEAL)
* [nlohmann/json](https://github.com/nlohmann/json)
* [Enron Email Dataset](https://www.cs.cmu.edu/~enron/)
