#include "seal/seal.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;
using namespace seal;

int main() {
    EncryptionParameters parms(scheme_type::bfv);
    parms.set_poly_modulus_degree(8192);
    parms.set_coeff_modulus(CoeffModulus::BFVDefault(8192));
    parms.set_plain_modulus(PlainModulus::Batching(8192, 20));

    SEALContext context(parms);
    BatchEncoder encoder(context);
    PublicKey public_key;

    ifstream pub_in("./build/public.key", ios::binary);
    if (!pub_in.is_open()) {
        cerr << "Could not open public.key\n";
        return 1;
    }
    public_key.load(context, pub_in);

    Encryptor encryptor(context, public_key);

    // Get the keyword from user
    string keyword;
    cout << "Enter keyword to encrypt: ";
    getline(cin, keyword);

    // Convert keyword to ASCII values
    vector<uint64_t> ascii_vec(encoder.slot_count(), 0);
    for (size_t i = 0; i < keyword.length(); ++i) {
        ascii_vec[i] = static_cast<uint64_t>(tolower(keyword[i]));
    }

    Plaintext plain;
    encoder.encode(ascii_vec, plain);

    Ciphertext encrypted_keyword;
    encryptor.encrypt(plain, encrypted_keyword);

    ofstream out("keyword.ctxt", ios::binary);
    encrypted_keyword.save(out);
    cout << "Encrypted keyword saved to keyword.ctxt\n";
    return 0;
}
