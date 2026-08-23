#pragma once
#include "tun/tun.hpp"
#include "config/data/tun_config.hpp"
#include "video_stream_in.hpp"
#include "video_stream_out.hpp"
#include <thread>

struct video_socket_settings {
    tun* tunnel;

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

    void _run_writer();
    void _run_reader();
public:
    video_socket(video_socket_settings &settings);
    tun *get_tun();
    video_stream_in *stream_in();
    video_stream_out *stream_out();
    void run();
    ~video_socket();
};