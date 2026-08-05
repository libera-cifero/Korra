#pragma once
#include "config/data/video_config.hpp"
#include "config/parser/parser.hpp"
#include "video_codec/frame_codec/provider/provider.hpp"

class provider_parser : public abstract_parser<provider*>, public contexted_in<video_config>
{
public:
    provider* parse(json) override;
    json serialize(provider*) override;  
};