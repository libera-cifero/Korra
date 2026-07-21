#pragma once
#include "config/data/video_config.hpp"
#include "provider/provider_parser.hpp"
#include "config/parser/parser.hpp"
#include "video/encoder/video_encoder.hpp"

class encoder_parser : public json_parser<video_encoder*>{
public:
    video_config *video;
    provider_parser *provider;
    video_encoder* parse(json) override;
    json serialize(video_encoder*) override;
};