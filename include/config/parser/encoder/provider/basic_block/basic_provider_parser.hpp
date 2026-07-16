#pragma once
#include "config/parser/encoder/provider/basic_block/color_codec/color_codec_parser.hpp"
#include "encoder/provider/provider.hpp"
#include "config/parser/parser.hpp"

class basic_provider_parser : public json_parser<provider*> {
public:
    color_codec_parser *color_codec;
    provider* parse(json) override;
    json serialize(provider*) override;
};