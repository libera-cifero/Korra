#include "config/data/cipher_config.hpp"
#include "config/parser/parser.hpp"
#include "video_codec/frame_codec/cipher/cipher.hpp"
#include <cryptopp/config.h>

using crypto_byte = CryptoPP::byte;

class xchacha20_cipher_parser : public specific_parser<cipher*>, public contexted_in<cipher_config> {
private:
    static int _get_seed(crypto_byte *key);
    static int _rand_int(int seed);

    crypto_byte *_generate_key(string password);
    crypto_byte *_generate_iv(crypto_byte *key);
    crypto_byte *_read_hex(string hex_str, int byte_size);
    string _write_hex(crypto_byte *bytes, int bytes_count);
public:
    cipher* parse(json) override;
    json serialize(cipher*) override;  
    string type() override;
    bool can_serialize(cipher*) override;
};