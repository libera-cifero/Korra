#pragma once

#include "config/data/cipher_config.hpp"
#include "config/parser/parser.hpp"
#include "video_codec/frame_codec/cipher/cipher.hpp"

class none_cipher_parser : public specific_parser<cipher*>, public contexted_in<cipher_config> {
public:
    cipher* parse(json) override;
    json serialize(cipher*) override;  
    string type() override;
    bool can_serialize(cipher*) override;
};