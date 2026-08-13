#include "config/data/cipher_config.hpp"
#include "video_codec/frame_codec/cipher/cipher.hpp"
#include <cryptopp/chachapoly.h>
#include <cryptopp/osrng.h>
#include <cryptopp/files.h>
#include <cryptopp/filters.h>
#include <cryptopp/secblock.h>

using namespace CryptoPP;

struct xchacha20_config : cipher_config {
    byte *key;
};

class xchacha20_cipher : public cipher {
private:
    AutoSeededRandomPool _seed_gen;
    XChaCha20Poly1305::Encryption _encyptor;
public:
    static const int KEY_SIZE = 32;
    static const int NONCE_SIZE = 24;
    static const int TAG_SIZE = 16;
    xchacha20_cipher(xchacha20_config *config);
    int header_size() override;
    char *encrypt(char *data) override;
    char *decrypt(char *encrypted) override;

    ~xchacha20_cipher();
};