#pragma once
#include "config/data/ffmpeg_rtmp_config.hpp"
#include "video_pipe_in.hpp"

class ffmpeg_rtmp_pipe_in : public video_pipe_in {
private:
    ffmpeg_rtmp_config _config;
    FILE *_pipe_in;
    int _frame_size();
protected:
    std::string __cmd;
public:
    ffmpeg_rtmp_pipe_in(ffmpeg_rtmp_config &config);
    char *read(int size) override;
    std::string url();
    ~ffmpeg_rtmp_pipe_in();
};