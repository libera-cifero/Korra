#pragma once
#include "video/pipe/video_pipe_out.hpp"
#include "config/parser/parser.hpp"

class ffmpeg_rtmp_pipe_out_parser : public specific_parser<video_pipe_out*> {
public:
    string type() override;
    video_pipe_out* parse(json) override;
    json serialize(video_pipe_out*) override;
};