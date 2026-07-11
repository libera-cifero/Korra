#include "video_pipe/ffmpeg_rtmp/ffmpeg_rtmp_pipe_out.hpp"
#include <cstdio>
#include <chrono>
#include <thread>

ffmpeg_rtmp_pipe_out::ffmpeg_rtmp_pipe_out(ffmpeg_rtmp_config config){
    _config = config;
    char command[1024];
    sprintf(command, 
        "ffmpeg -f rawvideo -pix_fmt bgr24 -s %dx%d -r %d -i pipe:0 "
        "-c:v libx264 -preset ultrafast -tune zerolatency "
        "-pix_fmt yuv420p -f flv %s",
        _config.frame_width, _config.frame_height, _config.fps, _config.rtmp_url.c_str());
    _pipe_out = popen(command, "w");
}

int ffmpeg_rtmp_pipe_out::_frame_size() { return _config.frame_height * _config.frame_width  * 3; }

void ffmpeg_rtmp_pipe_out::write(uint8_t *data, int size) {
    int frame_size = _frame_size();
    int frame_count = size / frame_size + (size % frame_size > 0 ? 1 : 0);
    auto frame_duration = std::chrono::milliseconds(1000 / _config.fps);
    auto next_frame_time = std::chrono::steady_clock::now();

    for(int i = 0; i < frame_count; i++) {
        fwrite(data, 1, frame_size, _pipe_out);
        fflush(_pipe_out);
        next_frame_time += frame_duration;
        data += frame_size;
        std::this_thread::sleep_until(next_frame_time);
    }
}

ffmpeg_rtmp_pipe_out::~ffmpeg_rtmp_pipe_out(){
    pclose(_pipe_out);
}