#pragma once
#include "video_pipe/video_pipe_in.hpp"
#include "config/parser/parser.hpp"

class ffmpeg_rtmp_pipe_in_parser : public specific_parser<video_pipe_in*> {
public:
    string type() override;
    video_pipe_in* parse(json) override;
    json serialize(video_pipe_in*) override;
};