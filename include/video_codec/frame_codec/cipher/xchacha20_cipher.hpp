#pragma once
#include "config/data/cipher_config.hpp"
#include "video_codec/frame_codec/cipher/cipher.hpp"
#include <cryptopp/chachapoly.h>
#include <cryptopp/osrng.h>
#include <cryptopp/files.h>
#include <cryptopp/filters.h>
#include <cryptopp/secblock.h>

struct xchacha20_settings : cipher_config {
    CryptoPP::byte *key;
    CryptoPP::byte *iv;

    ~xchacha20_settings() override {
        delete [] key;
        delete [] iv;
    }
};

class xchacha20_cipher : public cipher {
private:
    CryptoPP::AutoSeededRandomPool _seed_gen;
    CryptoPP::XChaCha20Poly1305::Encryption _encryptor;
    CryptoPP::XChaCha20Poly1305::Decryption _decryptor;
public:
    static const int KEY_SIZE = 32;
    static const int NONCE_SIZE = 24;
    static const int TAG_SIZE = 16;
    xchacha20_cipher(xchacha20_settings *config);
    int header_size() override;
    char *encrypt(char *data) override;
    char *decrypt(char *encrypted) override;
};