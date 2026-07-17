#pragma once
#include "config/data/root_config.hpp"
#include "video_pipe/video_pipe_out.hpp"
#include "config/parser/parser.hpp"

class video_pipe_out_parser : public abstract_parser<video_pipe_out*>{
public:
    video_config *video;
    video_pipe_out* parse(json) override;
    json serialize(video_pipe_out*) override;
};