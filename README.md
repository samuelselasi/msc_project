# HEKSM: Homomorphic Encryption-based Keyword Search Mechanism

<div align="center">

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Python](https://img.shields.io/badge/Python-3.8+-blue.svg)](https://www.python.org/downloads/)
[![C++](https://img.shields.io/badge/C++-17-00599C.svg)](https://isocpp.org/)
[![Microsoft SEAL](https://img.shields.io/badge/SEAL-4.1+-green.svg)](https://github.com/microsoft/SEAL)

**A Privacy-Preserving Framework for Digital Forensic Investigations**

[Overview](#overview) • [Features](#key-features) • [Installation](#installation) • [Usage](#usage) • [Results](#performance-evaluation) • [Citation](#citation)

</div>

---

## Overview

HEKSM implements a novel approach to digital forensic investigations that leverages **Homomorphic Encryption (HE)** to enable keyword searches on encrypted datasets without requiring decryption. Built on the Brakerski/Fan-Vercauteren (BFV) scheme via Microsoft SEAL, this framework addresses critical privacy concerns in third-party data handling during forensic examinations.

### Motivation

Traditional digital forensic workflows often expose sensitive data beyond the scope of investigation, violating privacy principles and legal frameworks like GDPR. HEKSM provides a cryptographically enforced solution that:

- **Eliminates plaintext exposure** during keyword searches
- **Maintains forensic integrity** while preserving privacy
- **Supports audit trails** for legal compliance
- **Balances investigative utility** with ethical data handling

This work is part of my MSc thesis at the University of Ghana, supervised by Dr. Edward Danso Ansong, exploring the intersection of cryptography, privacy-preserving computation, and digital forensics.

---

## Key Features

### **Cryptographic Privacy**
- End-to-end encryption using BFV homomorphic encryption scheme
- Zero-knowledge keyword searches without decryption
- Provable privacy guarantees backed by lattice-based cryptography

### **Efficient Search Operations**
- Encrypted dictionary indexing for O(1) keyword lookup
- Optimized SEAL parameter configuration for forensic-scale datasets
- Batch processing support for multiple keyword queries

### **Comprehensive Evaluation**
- Precision, recall, and F1-score benchmarking
- Memory footprint and computational overhead analysis
- Comparative studies against plaintext baselines

### **Academic Rigor**
- Reproducible experiments with open-source implementation
- Detailed documentation and methodology
- Alignment with privacy-preserving forensics principles

---

## Architecture

The framework consists of four primary components:

```
┌─────────────────┐     ┌──────────────────┐     ┌─────────────────┐
│   Dataset       │────▶│   Tokenization   │────▶│   Encryption    │
│  Preprocessing  │     │   & Indexing     │     │   (BFV/SEAL)    │
└─────────────────┘     └──────────────────┘     └─────────────────┘
                                                           │
                                                           ▼
┌─────────────────┐     ┌──────────────────┐     ┌─────────────────┐
│   Performance   │◀────│   Evaluation     │◀────│   Encrypted     │
│   Metrics       │     │   & Analysis     │     │   Search        │
└─────────────────┘     └──────────────────┘     └─────────────────┘
```

### Technology Stack

- **Cryptographic Library**: Microsoft SEAL 4.1+ (BFV scheme)
- **Backend Processing**: C++17 with optimized SEAL integration
- **Data Pipeline**: Python 3.8+ (Pandas, NumPy)
- **Evaluation Tools**: Matplotlib, Seaborn for visualization
- **Dataset**: Enron Email Corpus (subset of 1,000 emails)

---

## Installation

### Prerequisites

- **Operating System**: Linux (Ubuntu 20.04+) or macOS
- **Compiler**: GCC 9+ or Clang 10+ with C++17 support
- **Build Tools**: CMake 3.15+, Make
- **Python**: 3.8 or higher with pip

### Automated Setup

The repository includes an automated installation script that handles all dependencies:

```bash
# Clone the repository
git clone https://github.com/samuelselasi/heksm.git
cd heksm

# Run the comprehensive installation script
bash install_all.sh
```

This script performs the following operations:
1. Installs Python dependencies from `requirements.txt`
2. Installs C++ build tools and dependencies
3. Clones and builds Microsoft SEAL from source
4. Compiles all C++ components under `scripts/HE-scripts/`
5. Validates the installation with test runs

### Manual Installation

If you prefer manual setup or encounter issues:

```bash
# Install Python dependencies
pip install -r requirements.txt

# Install system dependencies (Ubuntu/Debian)
sudo apt-get update
sudo apt-get install -y build-essential cmake git libssl-dev

# Clone and build Microsoft SEAL
git clone https://github.com/microsoft/SEAL.git
cd SEAL
cmake -S . -B build -DSEAL_USE_MSGSL=OFF -DSEAL_USE_ZLIB=OFF -DSEAL_USE_ZSTD=OFF
cmake --build build
sudo cmake --install build

# Build HEKSM C++ components
cd ../scripts/HE-scripts
mkdir -p build && cd build
cmake ..
make
```

---

## Usage

### 1. Dataset Preparation

Preprocess the Enron email corpus and generate keyword indices:

```bash
# Clean and split the dataset
python3 scripts/splits/clean_n_split_1.py

# Extract top-N keywords for search queries
python3 scripts/splits/top_keywords.py
```

**Output**:
- `datasets/cleaned/split_emails.csv`: Cleaned email subset
- `scripts/splits/top_keywords.json`: High-frequency keyword list

### 2. Encryption Phase

Encrypt the dataset and build the secure index:

```bash
cd scripts/HE-scripts/build
./new_encryption
```

**Output**:
- `datasets/encrypted/indexed_encrypted_emails.json`: Encrypted email database
- `public.key`, `secret.key`: Cryptographic key pair
- `relin.key`: Relinearization keys for homomorphic operations

**Technical Details**:
- **Scheme**: BFV with polynomial modulus degree 8192
- **Plaintext Modulus**: 1024 (for ASCII encoding)
- **Coefficient Moduli**: 128-bit primes for security level λ=128

### 3. Encrypted Keyword Search

Execute homomorphic searches without decryption:

```bash
# Single keyword search
./new_search

# Batch keyword search (multiple queries)
python3 ../run_encrypted_searches.py
```

**Search Process**:
1. Keyword is encrypted using the public key
2. Homomorphic comparison operations are performed on ciphertexts
3. Matching indices are returned without decrypting the database
4. Results are decrypted only with the secret key

### 4. Baseline Comparison

Run plaintext searches for performance benchmarking:

```bash
cd ../../plaintext-scripts
python3 run_plaintext_searches.py
```

### 5. Performance Evaluation

Analyze accuracy, memory, and timing metrics:

```bash
cd ../analysis

# Compute precision, recall, F1-score
python3 precision_and_recall.py

# Measure memory usage for encrypted searches
bash measure_search_memory.sh

# Measure memory usage for plaintext searches
bash plaintext_memory_usage.sh

# Generate comparative visualizations
python3 plot_metrics.py
```

**Generated Reports**:
- `analysis/search_memory_usage.csv`: Encrypted search overhead
- `analysis/plaintext_memory_usage.csv`: Baseline memory consumption
- `analysis/precision_recall_plot.png`: Accuracy comparison
- `analysis/encrypted_results.json`: Detailed search outcomes

---

## Performance Evaluation

### Experimental Setup

- **Dataset**: 1,000 Enron emails (stratified sampling)
- **Hardware**: Intel Core i7-10700K, 32GB RAM
- **Keywords**: Top 10 most frequent terms (e.g., "meeting", "report", "schedule")
- **Metrics**: Precision, Recall, F1-Score, Search Time, Memory Usage

### Results Summary

| Metric                    | Encrypted Search | Plaintext Search | Overhead       |
|---------------------------|------------------|------------------|----------------|
| **Precision**             | 100%             | 100%             | 0%             |
| **Recall**                | 100%             | 100%             | 0%             |
| **F1-Score**              | 1.00             | 1.00             | 0%             |
| **Avg. Search Time**      | 89.9 seconds     | 0.50 seconds     | 179.8×         |
| **Memory per Query**      | ~114 MB          | ~8 MB            | 14.25×         |
| **Data Exposure Risk**    | **0%**           | **100%**         | **-100%**      |

### Key Findings

1. **Perfect Accuracy**: Encrypted search achieves identical precision and recall to plaintext baseline, demonstrating no loss of forensic utility.

2. **Acceptable Latency**: While encrypted searches incur a ~180× slowdown, the 89.9s average per query remains practical for forensic investigations, where privacy often outweighs speed.

3. **Memory Overhead**: The 14× memory increase is manageable on modern systems and scales linearly with dataset size.

4. **Privacy Guarantee**: Unlike plaintext methods that expose the entire database, HEKSM reveals zero information about non-matching records—a critical advantage for third-party data protection.

5. **Scalability Potential**: Performance can be improved through parameter optimization, GPU acceleration, and batching strategies (discussed in thesis).

---

## Project Structure

```
heksm/
├── datasets/
│   ├── original/                   # Raw Enron corpus
│   ├── cleaned/                    # Preprocessed data
│   │   ├── split_emails.csv        # Email subset
│   │   └── cleaned_split.csv       # Tokenization-ready version
│   └── encrypted/                  # Encrypted indices and databases
│       ├── indexed_encrypted_emails.json
│       ├── token_index_map.json
│       └── encrypted_data.json
│
├── scripts/
│   ├── splits/                     # Dataset preparation
│   │   ├── clean_n_split_1.py
│   │   └── top_keywords.py
│   ├── HE-scripts/                 # Homomorphic encryption (C++)
│   │   ├── new_encryption.cpp
│   │   ├── new_search.cpp
│   │   ├── CMakeLists.txt
│   │   └── seal/                   # SEAL headers
│   └── plaintext-scripts/          # Baseline implementations (Python)
│       ├── keyword_search_1.py
│       └── run_plaintext_searches.py
│
├── analysis/                       # Evaluation and visualization
│   ├── precision_and_recall.py
│   ├── measure_search_memory.sh
│   ├── plot_metrics.py
│   └── results/
│       ├── search_memory_usage.csv
│       └── encrypted_vs_plaintext.png
│
├── frontend/                       # Web interface (optional)
│   └── index.html
│
├── tools/                          # Utilities
│   └── clean.py
│
├── install_all.sh                  # Automated setup script
├── requirements.txt                # Python dependencies
├── LICENSE                         # MIT License
└── README.md                       # This file
```

---

## Reproducing Experiments

All experiments are fully reproducible. Follow the [Usage](#usage) section sequentially, or run the comprehensive test suite:

```bash
# Full pipeline (takes ~30 minutes)
bash run_full_experiment.sh
```

For custom dataset sizes, modify the loop limit in `scripts/HE-scripts/new_encryption.cpp`:

```cpp
// Line 142: Adjust email count
if (++email_count > 100) break;  // Change 100 to desired size
```

---

## Academic Context

### Publication Information

**Thesis Title**: *Securing Third-Party Data in Forensic Investigations Using Homomorphic Encryption*

**Author**: Samuel Selasi Kporvie

**Institution**: University of Ghana, Department of Computer Science

**Supervisor**: Dr. Edward Danso Ansong

**Year**: 2025

**Research Focus**: Privacy-preserving computation in digital forensics, applied cryptography, and ethical data handling in legal investigations.

### Theoretical Contributions

1. **Novel Framework**: First application of BFV homomorphic encryption to forensic keyword search with formal privacy analysis
2. **Performance Characterization**: Empirical evaluation of HE overheads in forensic contexts
3. **Legal Alignment**: Mapping of technical guarantees to GDPR and forensic standards

### Related Publications

- S. S. Kporvie and E. D. Ansong, "Homomorphic Encryption for Privacy-Preserving Forensics," *In Preparation*, 2025.

---

## Future Work

### Short-Term Enhancements
- **Multi-Keyword Search**: Boolean queries (AND/OR) on encrypted data
- **Fuzzy Matching**: Approximate string matching using Levenshtein distance
- **Phrase Search**: N-gram indexing for multi-word queries

### Long-Term Research Directions
- **Hardware Acceleration**: GPU-based HE operations using cuHE
- **Distributed Forensics**: Multi-party computation for collaborative investigations
- **Post-Quantum Security**: Transitioning to CRYSTALS-KYBER for lattice-based encryption

### Integration Opportunities
- **Forensic Tool Integration**: Plugins for Autopsy, EnCase, FTK
- **Cloud Deployment**: Kubernetes-based scalable architecture
- **Legal Compliance**: GDPR-compliant audit logging

---

## Privacy and Compliance

This framework is designed with legal and ethical considerations at its core:

- **GDPR Article 25**: Privacy by design and by default
- **NIST Privacy Framework**: De-identification and access control
- **Forensic Standards**: ISO/IEC 27037 for digital evidence handling
- **Cryptographic Security**: Lattice-based encryption (post-quantum safe)

**Use Case Alignment**: Lawful interception, e-discovery, corporate investigations, and academic research where third-party privacy is paramount.

---

## Contributing

Contributions are welcome! If you're interested in extending this work:

1. **Fork** the repository
2. **Create** a feature branch (`git checkout -b feature/enhanced-search`)
3. **Commit** your changes (`git commit -m 'Add fuzzy matching'`)
4. **Push** to the branch (`git push origin feature/enhanced-search`)
5. **Open** a Pull Request

For major changes, please open an issue first to discuss proposed modifications.

---

## License

This project is licensed under the [MIT License](LICENSE), which permits academic and commercial use with attribution.

**Citation Requirement**: If you use this work in academic publications, please cite as follows:

```bibtex
@mastersthesis{kporvie2025heksm,
  author       = {Samuel Selasi Kporvie},
  title        = {Securing Third-Party Data in Forensic Investigations Using Homomorphic Encryption},
  school       = {University of Ghana},
  year         = {2025},
  type         = {{MSc} Thesis},
  url          = {https://github.com/samuelselasi/heksm}
}
```

---

## Acknowledgments

This research was made possible through:

- **Supervision**: Dr. Edward Danso Ansong (University of Ghana)
- **Frontend Development**: [Eugene Agyei Osae](https://github.com/quameEugene)
- **Dataset**: [Enron Email Corpus](https://www.cs.cmu.edu/~enron/) (Carnegie Mellon University)
- **Cryptographic Library**: [Microsoft SEAL](https://github.com/microsoft/SEAL)
- **Institutional Support**: University of Ghana Computer Science Department

Special thanks to the open-source community for tools that made this research possible.

---

## Contact

For academic inquiries, collaboration opportunities, or technical questions:

- **Email**: [sskporvie001@st.ug.edu.gh](mailto:sskporvie001@st.ug.edu.gh)
- **GitHub Issues**: [Report bugs or request features](https://github.com/samuelselasi/heksm/issues)
- **LinkedIn**: [Connect for research discussions](https://linkedin.com/in/samuelselasi)

I'm actively seeking PhD positions in **applied cryptography**, **privacy-preserving computation**, and **secure systems**. Feel free to reach out for discussions on research directions or potential collaborations.

---

<div align="center">

</div>
