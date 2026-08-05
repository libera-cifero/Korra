#pragma once
#include "parser.hpp"
#include "config/data/video_config.hpp"

class video_config_parser : public json_parser<video_config> {
public:
    video_config parse(json) override;
    json serialize(video_config) override;
};