#pragma once
#include "ffmpeg_rtmp_settings.hpp"
#include "video/pipe/video_pipe_out.hpp"

class ffmpeg_rtmp_pipe_out : public video_pipe_out {
private:
    ffmpeg_rtmp_settings _config;
    FILE *_pipe_out;

    int _frame_size();
public:
    ffmpeg_rtmp_pipe_out(ffmpeg_rtmp_settings config);
    void write(uint8_t *data, int size) override;
    ~ffmpeg_rtmp_pipe_out();
};