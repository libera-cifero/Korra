#pragma once
#include "pipe/video_pipe_in.hpp"
#include "pipe/video_pipe_out.hpp"
#include "video_codec/video_codec.hpp"
#include "data_boxer/data_boxer.hpp"
#include "data_boxer/data_unboxer.hpp"
#include "tun/tun.hpp"

class video_socket {
private:
    data_boxer *_boxer = nullptr;
    data_unboxer *_unboxer = nullptr;
    video_codec *_codec = nullptr;
    video_pipe_in *_pipe_in = nullptr;
    video_pipe_out *_pipe_out = nullptr;
    tun *_tun = nullptr;
public:
    video_socket(data_boxer *boxer, data_unboxer *unboxer, video_codec *codec, video_pipe_in *pipe_in, video_pipe_out *pipe_out);
    void run();
    ~video_socket();
};