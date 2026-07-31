#pragma once
#include "ffmpeg_rtmp/ffmpeg_rtmp_settings.hpp"
#include "video_pipe_in.hpp"

class ffmpeg_rtmp_pipe_in : public video_pipe_in {
private:
    ffmpeg_rtmp_settings _config;
    FILE *_pipe_in;
    int _frame_size();
public:
    ffmpeg_rtmp_pipe_in(ffmpeg_rtmp_settings config);
    uint8_t *read(int size) override;
    ~ffmpeg_rtmp_pipe_in();
};