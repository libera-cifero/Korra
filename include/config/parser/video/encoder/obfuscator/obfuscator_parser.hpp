#include "config/parser/parser.hpp"
#include "video_encoder/cipher/cipher.hpp"
class cipher_parser : public abstract_parser<cipher*> {
public:
    cipher* parse(json) override;
    json serialize(cipher*) override;
};