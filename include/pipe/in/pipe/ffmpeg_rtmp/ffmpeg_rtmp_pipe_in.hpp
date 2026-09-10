#pragma once
#include "config/data/ffmpeg_rtmp_config.hpp"
#include "../video_pipe_in.hpp"

class ffmpeg_rtmp_pipe_in : public video_pipe_in {
protected:
    FILE *__pipe;
    ffmpeg_rtmp_config __config;
    std::string __cmd;
    int __frame_size;
    char *__buffer;
    int __bytes_readed;
    virtual int __read_async() = 0;
public:
    ffmpeg_rtmp_pipe_in(ffmpeg_rtmp_config &config);
    char *read(int size) override;
    std::string url();
    ~ffmpeg_rtmp_pipe_in();
};