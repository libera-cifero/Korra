#pragma once
#include "video_pipe/video_pipe_in.hpp"
#include "config/parser/parser.hpp"

class ffmpeg_rtmp_pipe_in_parser : public json_parser<video_pipe_in*> {
public:
    video_pipe_in* parse(json) override;
    json serialize(video_pipe_in*) override;
};