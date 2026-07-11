#include "video_pipe/ffmpeg_rtmp/ffmpeg_rtmp_pipe_in.hpp"
#include <cstdint>
#include <cstdio>
#include <chrono>
#include <thread>

ffmpeg_rtmp_pipe_in::ffmpeg_rtmp_pipe_in(ffmpeg_rtmp_config config){
    _config = config;
    char command[256];
    sprintf(command, "ffmpeg -i %s -f rawvideo -pix_fmt bgr24 -an pipe:1", config.rtmp_url.c_str());
    _pipe_in = popen(command, "r");
}

int ffmpeg_rtmp_pipe_in::_frame_size() { return _config.frame_height * _config.frame_width  * 3; }

uint8_t *ffmpeg_rtmp_pipe_in::read(int size) {
    uint8_t *data=new uint8_t[size], *ptr;
    int frame_size = _frame_size();
    int frame_count = size / frame_size + (size % frame_size > 0 ? 1 : 0);
    auto frame_duration = std::chrono::milliseconds(1000 / _config.fps);
    auto next_frame_time = std::chrono::steady_clock::now();

    for(int i = 0; i < frame_count; i++) {
        ptr = data + i * frame_size;
        fread(ptr, 1, frame_size, _pipe_in);
        next_frame_time += frame_duration;
        std::this_thread::sleep_until(next_frame_time);
    }
    
    return data;
}

ffmpeg_rtmp_pipe_in::~ffmpeg_rtmp_pipe_in(){
    pclose(_pipe_in);
}