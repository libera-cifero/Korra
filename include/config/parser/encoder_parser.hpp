#pragma once
#include "config/data/encoder_config.hpp"
#include "config/parser/encoder/provider/provider_parser.hpp"
#include "config/parser/parser.hpp"
#include "encoder/video_encoder.hpp"

class encoder_parser : public json_parser<video_encoder*>{
public:
    provider_parser *provider;
    video_encoder* parse(json) override;
    json serialize(video_encoder*) override;
};