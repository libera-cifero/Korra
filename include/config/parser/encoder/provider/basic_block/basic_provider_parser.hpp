#pragma once
#include "config/data/root_config.hpp"
#include "config/parser/encoder/provider/basic_block/color_codec/color_codec_parser.hpp"
#include "video/encoder/provider/provider.hpp"
#include "config/parser/parser.hpp"

class basic_provider_parser : public specific_parser<provider*> {
public:
    video_config *config;
    color_codec_parser *color_codec;
    string type() override;
    provider* parse(json) override;
    json serialize(provider*) override;
};