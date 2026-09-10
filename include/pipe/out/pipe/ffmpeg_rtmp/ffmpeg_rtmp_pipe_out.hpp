#pragma once
#include "config/data/ffmpeg_rtmp_config.hpp"
#include "../video_pipe_out.hpp"

class ffmpeg_rtmp_pipe_out : public video_pipe_out {
protected:
    ffmpeg_rtmp_config __config;
    FILE *__pipe_out;
    std::string __cmd;

    int __frame_size();
public:
    ffmpeg_rtmp_pipe_out(ffmpeg_rtmp_config &config);
    std::string url();
};