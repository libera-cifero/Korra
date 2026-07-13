#include "color_codec/color_codec.hpp"
#include "config/parser/parser.hpp"

class color_codec_parser : public json_parser<color_codec*> {
public:
    virtual color_codec* parse(json) override;
    virtual json serialize(color_codec*) override;
};