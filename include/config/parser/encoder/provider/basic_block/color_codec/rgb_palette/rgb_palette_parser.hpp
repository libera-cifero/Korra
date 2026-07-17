#pragma once
#include "color_codec/color_codec.hpp"
#include "config/parser/parser.hpp"

class rgb_palette_parser : public specific_parser<color_codec*> {
public:
    string type() override;
    color_codec* parse(json) override;
    json serialize(color_codec*) override;
};