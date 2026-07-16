#pragma once
#include "color_codec/color_codec.hpp"
#include "config/parser/parser.hpp"

class color_codec_parser : public abstract_parser<color_codec*> {
public:
    color_codec* parse(json) override;
    json serialize(color_codec*) override;
};