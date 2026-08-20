#pragma once
#include "pipe/in/pipe/video_pipe_in.hpp"
#include "pipe/out/pipe/video_pipe_out.hpp"
#include "video_codec/video_codec.hpp"
#include "data_boxer/data_boxer.hpp"
#include "data_boxer/data_unboxer.hpp"
#include "tun/tun.hpp"
#include "config/data/tun_config.hpp"
#include <mutex>
#include <semaphore>
#include <thread>

struct video_socket_settings {
    data_boxer *boxer; 
    data_unboxer *unboxer; 
    video_codec *codec; 
    video_pipe_in *pipe_in; 
    video_pipe_out *pipe_out;

    tun_config tun;
};

class video_socket {
private:
    data_boxer *_boxer = nullptr;
    data_unboxer *_unboxer = nullptr;
    video_codec *_codec = nullptr;
    video_pipe_in *_pipe_in = nullptr;
    video_pipe_out *_pipe_out = nullptr;
    tun *_tun = nullptr;
    
    thread _thread_writer;
    thread _thread_reader;
    thread _thread_codec;
    char *_frame_to_write;
    mutex _frame_to_write_mutex;
    binary_semaphore _codec_writer_sync = binary_semaphore(0);

    bool _is_running = false;
    bool _frame_updated = false;

    void _run_writer();
    void _run_reader();
    void _run_codec_handler();
public:
    video_socket(video_socket_settings &settings);
    void run();
    ~video_socket();
};