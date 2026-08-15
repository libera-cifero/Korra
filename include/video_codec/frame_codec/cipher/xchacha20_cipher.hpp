#include "config/data/cipher_config.hpp"
#include "video_codec/frame_codec/cipher/cipher.hpp"
#include <cryptopp/chachapoly.h>
#include <cryptopp/osrng.h>
#include <cryptopp/files.h>
#include <cryptopp/filters.h>
#include <cryptopp/secblock.h>

using crypto_byte = CryptoPP::byte;

struct xchacha20_settings : cipher_config {
    crypto_byte *key;
    crypto_byte *iv;
};

class xchacha20_cipher : public cipher {
private:
    crypto_byte *_key;
    crypto_byte *_iv;
    CryptoPP::AutoSeededRandomPool _seed_gen;
    CryptoPP::XChaCha20Poly1305::Encryption _encryptor;
    CryptoPP::XChaCha20Poly1305::Decryption _decryptor;
public:
    static const int KEY_SIZE = 32;
    static const int NONCE_SIZE = 24;
    static const int TAG_SIZE = 16;
    xchacha20_cipher(xchacha20_settings &config);
    int header_size() override;
    crypto_byte *key();
    crypto_byte *iv();
    char *encrypt(char *data) override;
    char *decrypt(char *encrypted) override;

    ~xchacha20_cipher();
};