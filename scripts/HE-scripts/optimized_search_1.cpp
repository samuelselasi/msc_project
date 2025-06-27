#include "seal/seal.h"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include <sstream>
#include <chrono>

using namespace std;
using namespace seal;
using json = nlohmann::json;

string sanitize(const string& text) {
    string cleaned;
    for (char c : text) {
        if (isalnum(c)) cleaned += tolower(c);
    }
    return cleaned;
}

string from_base64(const string &in) {
    static const string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    vector<int> T(256, -1);
    for (int i = 0; i < 64; i++) T[chars[i]] = i;

    string out;
    int val = 0, valb = -8;
    for (unsigned char c : in) {
        if (T[c] == -1) break;
        val = (val << 6) + T[c];
        valb += 6;
        if (valb >= 0) {
            out.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return out;
}

vector<uint64_t> encode_word(const string& word, size_t slot_count) {
    vector<uint64_t> result(slot_count, 0);
    for (size_t i = 0; i < word.size() && i < slot_count; ++i)
        result[i] = static_cast<uint64_t>(word[i]);
    return result;
}

int main() {
    auto start_time = chrono::high_resolution_clock::now();

    EncryptionParameters parms(scheme_type::bfv);
    parms.set_poly_modulus_degree(8192);
    parms.set_coeff_modulus(CoeffModulus::BFVDefault(8192));
    parms.set_plain_modulus(PlainModulus::Batching(8192, 20));

    SEALContext context(parms);
    BatchEncoder encoder(context);
    size_t slot_count = encoder.slot_count();

    SecretKey secret_key;
    PublicKey public_key;
    ifstream pub_in("./build/public.key", ios::binary);
    ifstream sec_in("./build/secret.key", ios::binary);
    public_key.load(context, pub_in);
    secret_key.load(context, sec_in);

    Encryptor encryptor(context, public_key);
    Decryptor decryptor(context, secret_key);
    Evaluator evaluator(context);

    cout << "Enter keyword to search: ";
    string keyword;
    getline(cin, keyword);
    keyword = sanitize(keyword);

    // Encode and encrypt keyword
    Plaintext keyword_pt;
    encoder.encode(encode_word(keyword, slot_count), keyword_pt);
    Ciphertext keyword_ctxt;
    encryptor.encrypt(keyword_pt, keyword_ctxt);

    // Load data
    ifstream in("../../datasets/encrypted/indexed_encrypted_emails.json");
    json data;
    in >> data;

    const auto& encrypted_dict = data["dictionary"];
    const auto& emails = data["emails"];

    unordered_map<string, Ciphertext> dict_ciphertexts;

    for (auto& [word, base64_enc] : encrypted_dict.items()) {
        string raw = from_base64(base64_enc.get<string>());
        stringstream ss(raw);
        Ciphertext ctxt;
        ctxt.load(context, ss);
        dict_ciphertexts[word] = ctxt;
    }

    // Load messages for match display
    unordered_map<string, string> file_to_message;
    ifstream csv("../../datasets/cleaned/cleaned_split-1.csv");
    string header, row;
    getline(csv, header);
    while (getline(csv, row)) {
        stringstream ss(row);
        string email_id, from, to, cc, date, subject, body, message;
        getline(ss, email_id, ',');
        getline(ss, from, ',');
        getline(ss, to, ',');
        getline(ss, cc, ',');
        getline(ss, date, ',');
        getline(ss, subject, ',');
        getline(ss, body, ',');
        getline(ss, message);
        file_to_message[email_id] = message;
    }

    int match_count = 0;
    for (const auto& email : emails) {
        string file = email["file"];
        auto tokens = email["tokens"];

        for (const string& token : tokens) {
            const Ciphertext& token_ctxt = dict_ciphertexts[token];

            Ciphertext diff;
            evaluator.sub(token_ctxt, keyword_ctxt, diff);
            evaluator.square_inplace(diff);

            Plaintext result_pt;
            decryptor.decrypt(diff, result_pt);
            vector<uint64_t> decoded;
            encoder.decode(result_pt, decoded);

            bool is_match = all_of(decoded.begin(), decoded.end(), [](uint64_t v) { return v == 0; });
            if (is_match) {
                cout << "Match in file: " << file << endl;
                cout << "Message: " << file_to_message[file] << "\n" << endl;
                ++match_count;
                break;
            }
        }
    }

    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
    cout << "Done. Total matches: " << match_count << endl;
    cout << "Search time: " << duration.count() << " ms\n";
    return 0;
}
