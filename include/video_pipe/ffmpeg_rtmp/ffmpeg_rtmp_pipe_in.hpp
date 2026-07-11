#pragma once
#include "ffmpeg_rtmp_config.hpp"
#include "video_pipe/video_pipe_in.hpp"

class ffmpeg_rtmp_pipe_in : public video_pipe_in {
private:
    ffmpeg_rtmp_config _config;
    FILE *_pipe_in;
    int _frame_size();
public:
    ffmpeg_rtmp_pipe_in(ffmpeg_rtmp_config config);
    uint8_t *read(int size) override;
    ~ffmpeg_rtmp_pipe_in();
};