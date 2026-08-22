#pragma once
#include "config/data/video_config.hpp"
#include "pipe/in/pipe/video_pipe_in.hpp"
#include "pipe/out/pipe/video_pipe_out.hpp"
#include "video_codec/video_codec.hpp"
#include "data_boxer/data_boxer.hpp"
#include "data_boxer/data_unboxer.hpp"
#include "tun/tun.hpp"
#include "config/data/tun_config.hpp"
#include "video_stream_in.hpp"
#include "video_stream_out.hpp"
#include <mutex>
#include <semaphore>
#include <thread>

struct video_socket_settings {
    tun* tun;

    video_stream_in *stream_in;
    video_stream_out *stream_out;
};

class video_socket {
private:
    video_stream_in *_stream_in = nullptr;
    video_stream_out *_stream_out = nullptr;
    tun *_tun = nullptr;

    thread _reader_thread;
    thread _writer_thread;

    bool _is_running = false;
    bool _frame_updated = false;

    void _run_writer();
    void _run_reader();
    void _run_codec_handler();
public:
    video_socket(video_socket_settings &settings);
    tun *tun();
    video_stream_in *stream_in();
    video_stream_out *stream_out();
    void run();
    ~video_socket();
};