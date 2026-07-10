#pragma once
#include "ffmpeg_rtmp_config.hpp"
#include "encoder/provider/video_pipe/video_pipe_out.hpp"

class ffmpeg_rtmp_pipe_out : public video_pipe_out {
private:
    ffmpeg_rtmp_config _config;
public:
    ffmpeg_rtmp_pipe_out(ffmpeg_rtmp_config config);
    void write(uint8_t *data, int size) override;
    ~ffmpeg_rtmp_pipe_out();
};