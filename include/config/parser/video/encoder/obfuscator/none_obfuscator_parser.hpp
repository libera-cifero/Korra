#include "config/parser/parser.hpp"
#include "video_encoder/cipher/cipher.hpp"
class none_cipher_parser : public specific_parser<cipher*> {
public:
    string type() override;
    cipher* parse(json) override;
    json serialize(cipher*) override;
};