#pragma once
#include "config/data/video_config.hpp"
#include "data_boxer/data/korra_data.hpp"
#include "data_boxer/data_unboxer.hpp"
#include "pipe/in/pipe/video_pipe_in.hpp"
#include "video_codec/video_codec.hpp"

class video_stream_in {
private:
    video_pipe_in *_pipe;
    video_codec *_codec;
    data_unboxer *_unboxer;
    video_config _config;
public:
    video_stream_in(video_config &config, video_pipe_in *pipe, video_codec *codec, data_unboxer *unboxer);
    video_config config();
    void read(vector<korra_data*> &buffer);
    ~video_stream_in();
};