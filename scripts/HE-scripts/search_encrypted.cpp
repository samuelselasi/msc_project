#include "seal/seal.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include <sstream>

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

    cout << "🔑 Enter keyword to search: ";
    string keyword;
    getline(cin, keyword);
    keyword = sanitize(keyword);

    vector<uint64_t> keyword_vec = encode_word(keyword, slot_count);
    Plaintext keyword_pt;
    encoder.encode(keyword_vec, keyword_pt);
    Ciphertext keyword_ctxt;
    encryptor.encrypt(keyword_pt, keyword_ctxt);

    // Load encrypted tokenized dataset
    ifstream in("../../datasets/encrypted/tokenized_emails.json");
    json encrypted_data;
    in >> encrypted_data;

    int match_count = 0;
    for (auto& email : encrypted_data) {
        string filename = email["file"];
        auto tokens = email["tokens"];

        for (const auto& token_b64 : tokens) {
            stringstream ss(token_b64.get<string>());
            Ciphertext token_ctxt;
            token_ctxt.load(context, ss);

            Ciphertext diff;
            evaluator.sub(token_ctxt, keyword_ctxt, diff);
            evaluator.square_inplace(diff);

            Plaintext result_pt;
            decryptor.decrypt(diff, result_pt);
            vector<uint64_t> decoded;
            encoder.decode(result_pt, decoded);

            bool is_match = all_of(decoded.begin(), decoded.end(), [](uint64_t v) { return v == 0; });
            if (is_match) {
                cout << "✅ Match in file: " << filename << endl;
                ++match_count;
                break;
            }
        }
    }

    cout << "\n🔍 Done. Total matches: " << match_count << endl;
    return 0;
}
