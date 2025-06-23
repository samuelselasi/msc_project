#include "seal/seal.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <nlohmann/json.hpp>  // You’ll need this header

using namespace std;
using namespace seal;
using json = nlohmann::json;

string sanitize(const string& text) {
    string cleaned;
    for (char c : text) {
        if (isalnum(c) || c == ' ') cleaned += tolower(c);
    }
    return cleaned;
}

vector<string> tokenize(const string& text) {
    vector<string> tokens;
    stringstream ss(text);
    string word;
    while (ss >> word) {
        tokens.push_back(word);
    }
    return tokens;
}

vector<uint64_t> encode_word(const string& word, size_t slot_count) {
    vector<uint64_t> result(slot_count, 0);
    for (size_t i = 0; i < word.size() && i < slot_count; ++i) {
        result[i] = static_cast<uint64_t>(word[i]);
    }
    return result;
}

int main() {
    EncryptionParameters parms(scheme_type::bfv);
    parms.set_poly_modulus_degree(8192);
    parms.set_coeff_modulus(CoeffModulus::BFVDefault(8192));
    parms.set_plain_modulus(PlainModulus::Batching(8192, 20));

    SEALContext context(parms);
    KeyGenerator keygen(context);
    // PublicKey public_key = keygen.public_key();
    PublicKey public_key;
    keygen.create_public_key(public_key);
    SecretKey secret_key = keygen.secret_key();
    Encryptor encryptor(context, public_key);
    BatchEncoder encoder(context);

    size_t slot_count = encoder.slot_count();

    ofstream pub_out("./build/public.key", ios::binary);
    ofstream sec_out("./build/secret.key", ios::binary);
    public_key.save(pub_out);
    secret_key.save(sec_out);

    ifstream csv("../../datasets/cleaned/cleaned-split-1.csv");
    string header, line;
    getline(csv, header); // Skip header

    ofstream out("../../datasets/encrypted/tokenized_emails.json");
    json encrypted_data = json::array();

    while (getline(csv, line)) {
        stringstream ss(line);
        string file, message;

        getline(ss, file, ',');
        getline(ss, message); // Message might contain commas

        message = sanitize(message);
        vector<string> words = tokenize(message);

        json email_entry;
        email_entry["file"] = file;
        json word_ctxts = json::array();

        for (const auto& word : words) {
            Plaintext pt;
            encoder.encode(encode_word(word, slot_count), pt);
            Ciphertext ctxt;
            encryptor.encrypt(pt, ctxt);

            stringstream ss;
            ctxt.save(ss);
            word_ctxts.push_back(ss.str());
        }

        email_entry["tokens"] = word_ctxts;
        encrypted_data.push_back(email_entry);
    }

    out << encrypted_data.dump(2);
    cout << "Tokenized and encrypted all emails.\n";
    return 0;
}
