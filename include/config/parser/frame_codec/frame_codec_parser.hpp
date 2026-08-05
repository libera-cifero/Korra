#pragma once
#include "config/data/video_config.hpp"
#include "config/parser/parser.hpp"
#include "cipher/cipher_parser.hpp"
#include "provider/provider_parser.hpp"
#include "video_codec/frame_codec/frame_codec.hpp"

class frame_codec_parser : public json_parser<frame_codec*>, public contexted_in<video_config> {
private:
    provider_parser *_provider_parser;
    cipher_parser *_cipher_parser;

    void _update_cipher_config(int payload_size);
public:
    frame_codec_parser(provider_parser *,cipher_parser *);
    frame_codec* parse(json) override;
    json serialize(frame_codec*) override;
    ~frame_codec_parser();
};