#pragma once
#include "data_boxer/data/korra_data.hpp"
#include "pipe/out/pipe/video_pipe_out.hpp"
#include "video_codec/video_codec.hpp"
#include "data_boxer/data_boxer.hpp"

class video_stream_out {
private:
    video_pipe_out *_pipe;
    video_codec *_codec;
    data_boxer *_boxer;
public:
    video_stream_out(video_pipe_out *pipe, video_codec *codec, data_boxer *boxer);
    void send_current_frame();
    void write(korra_data *data);
    ~video_stream_out();
};