#pragma once
#include "event/async_read_event.hpp"
#include "event/timer_event.hpp"
#include "tun/tun.hpp"
#include "config/data/tun_config.hpp"
#include "event/event_loop.hpp"
#include "video_stream_in.hpp"
#include "video_stream_out.hpp"

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
    event_loop *_loop = nullptr;

    void _on_tun_read_event(async_read_event_args *args);
    void _on_pipe_in_read_event(async_read_event_args *args);
    void _on_timer_event(timer_event_args *args);

    void _setup_event_loop(video_socket_settings &settings);
public:
    video_socket(video_socket_settings &settings);
    tun *get_tun();
    video_stream_in *stream_in();
    video_stream_out *stream_out();
    void run();
    void stop();
    ~video_socket();
};