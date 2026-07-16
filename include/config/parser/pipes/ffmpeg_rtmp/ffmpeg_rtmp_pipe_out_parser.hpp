#pragma once
#include "video_pipe/video_pipe_out.hpp"
#include "config/parser/parser.hpp"

class ffmpeg_rtmp_pipe_out_parser : public json_parser<video_pipe_out*> {
public:
    video_pipe_out* parse(json) override;
    json serialize(video_pipe_out*) override;
};