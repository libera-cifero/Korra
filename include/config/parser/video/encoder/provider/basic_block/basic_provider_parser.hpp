#pragma once
#include "config/data/video_config.hpp"
#include "color_codec/color_codec_parser.hpp"
#include "video_codec/frame_codec/provider/provider.hpp"
#include "config/parser/parser.hpp"

class basic_provider_parser : public specific_parser<provider*> {
public:
    video_config *config;
    color_codec_parser *color_codec;
    string type() override;
    provider* parse(json) override;
    json serialize(provider*) override;
};