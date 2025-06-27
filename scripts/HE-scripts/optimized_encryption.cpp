#include "seal/seal.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <iomanip>
#include <chrono>


using namespace std;
using namespace seal;
using json = nlohmann::json;

string to_base64(const string &input) {
    static const string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    string result;
    int val = 0, valb = -6;
    for (unsigned char c : input) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            result.push_back(chars[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) result.push_back(chars[((val << 8) >> (valb + 8)) & 0x3F]);
    while (result.size() % 4) result.push_back('=');
    return result;
}

string sanitize(const string& text) {
    string cleaned;
    for (char c : text) if (isalnum(c) || c == ' ') cleaned += tolower(c);
    return cleaned;
}

vector<string> tokenize(const string& text) {
    vector<string> tokens;
    stringstream ss(text);
    string word;
    while (ss >> word) tokens.push_back(word);
    return tokens;
}

vector<uint64_t> encode_word(const string& word, size_t slot_count) {
    vector<uint64_t> result(slot_count, 0);
    for (size_t i = 0; i < word.size() && i < slot_count; ++i)
        result[i] = static_cast<uint64_t>(word[i]);
    return result;
}

int main() {
    EncryptionParameters parms(scheme_type::bfv);
    parms.set_poly_modulus_degree(8192);
    parms.set_coeff_modulus(CoeffModulus::BFVDefault(8192));
    parms.set_plain_modulus(PlainModulus::Batching(8192, 20));

    SEALContext context(parms);
    KeyGenerator keygen(context);
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

    ifstream csv("../../datasets/cleaned/cleaned_split-1.csv");
    string header, line;
    getline(csv, header);

    unordered_map<string, string> encrypted_dict;
    json encrypted_data = json::array();
    int email_count = 0;

    auto start_time = chrono::high_resolution_clock::now();

    while (getline(csv, line)) {
        if (++email_count > 5) break;
        stringstream ss(line);
        string file, from, to, cc, date, subject, body, message;
        getline(ss, file, ',');
        getline(ss, from, ',');
        getline(ss, to, ',');
        getline(ss, cc, ',');
        getline(ss, date, ',');
        getline(ss, subject, ',');
        getline(ss, body, ',');
        getline(ss, message);

        string content = sanitize(subject + " " + body + " " + message);
        content.erase(remove_if(content.begin(), content.end(), [](char c) {
            return static_cast<unsigned char>(c) > 127;
        }), content.end());

        vector<string> words = tokenize(content);
        json word_ctxts = json::array();

        for (const auto& word : words) {
            if (encrypted_dict.find(word) == encrypted_dict.end()) {
                Plaintext pt;
                encoder.encode(encode_word(word, slot_count), pt);
                Ciphertext ctxt;
                encryptor.encrypt(pt, ctxt);
                stringstream ss;
                ctxt.save(ss);
                encrypted_dict[word] = to_base64(ss.str());
            }
            word_ctxts.push_back(encrypted_dict[word]);
        }

        json email_entry;
        email_entry["file"] = file;
        email_entry["tokens"] = word_ctxts;
        encrypted_data.push_back(email_entry);
    }

    ofstream out("../../datasets/encrypted/tokenized_emails.json");
    out << encrypted_data.dump(2);

    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end_time - start_time;
    cout << "Encryption time: " << duration.count() << " seconds\n";


    cout << "Processed " << email_count << " emails.\n";
    return 0;
}
