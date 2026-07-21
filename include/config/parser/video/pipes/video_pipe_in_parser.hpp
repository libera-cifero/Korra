#pragma once
#include "config/data/video_config.hpp"
#include "video/pipe/video_pipe_in.hpp"
#include "config/parser/parser.hpp"

class video_pipe_in_parser : public abstract_parser<video_pipe_in*>{
public:
    video_config *video;
    video_pipe_in* parse(json) override;
    json serialize(video_pipe_in*) override;
};