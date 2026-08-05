#pragma once
#include "config/data/video_config.hpp"
#include "parser.hpp"
#include "video_codec/frame_codec/frame_codec.hpp"

class frame_codec_parser : public json_parser<frame_codec*>, public contexted<video_config> {
public:
    
};