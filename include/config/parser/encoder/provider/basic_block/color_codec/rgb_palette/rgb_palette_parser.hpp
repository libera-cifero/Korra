#include "../color_codec_parser.hpp"
#include "color_codec/color_codec.hpp"

class rgb_palette_parser : public color_codec_parser {
public:
    color_codec* parse(json) override;
    json serialize(color_codec*) override;
};