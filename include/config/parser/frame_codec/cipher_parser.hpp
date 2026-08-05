#include "config/data/video_config.hpp"
#include "config/parser/parser.hpp"
#include "video_codec/frame_codec/cipher/cipher.hpp"

class cipher_parser : public abstract_parser<cipher*>, public contexted<video_config> {
public:
    cipher* parse(json) override;
    json serialize(cipher*) override;
};