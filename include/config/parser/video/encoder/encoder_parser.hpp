#pragma once
#include "config/data/video_config.hpp"
#include "provider/provider_parser.hpp"
#include "config/parser/parser.hpp"
#include "video_codec/video_codec.hpp"

class encoder_parser : public json_parser<video_codec*>{
public:
    video_config *video;
    provider_parser *provider;
    video_codec* parse(json) override;
    json serialize(video_codec*) override;
};