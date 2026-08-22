#pragma once
#include "config/parser/parser.hpp"
#include "pipe/out/pipe/video_pipe_out.hpp"
#include "config/data/video_config.hpp"

class ffmpeg_rtmp_pipe_out_parser : public specific_parser<video_pipe_out*>, public contexted_in<video_config> {
public:
    string type() override;
    bool can_serialize(video_pipe_out *) override;
    video_pipe_out *parse(json) override;
    json serialize(video_pipe_out*) override;
};