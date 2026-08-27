#include "pipe/out/pipe/ffmpeg_rtmp_pipe_out.hpp"
#include <cstdio>
#include <chrono>
#include <thread>

ffmpeg_rtmp_pipe_out::ffmpeg_rtmp_pipe_out(ffmpeg_rtmp_config &config){
    _config = config;
    char command[1024];
    sprintf(command, 
        "ffmpeg -f rawvideo -pix_fmt bgr24 -s %dx%d -r %d -i pipe:0 "
        "-c:v libx264 -preset ultrafast -tune zerolatency "
        "-pix_fmt yuv420p -f flv %s",
        _config.frame_width, _config.frame_height, _config.fps, _config.rtmp_url.c_str());
    _pipe_out = popen(command, "w");
}

std::string ffmpeg_rtmp_pipe_out::url(){
    return _config.rtmp_url;
}

int ffmpeg_rtmp_pipe_out::_frame_size() { return _config.frame_height * _config.frame_width  * 3; }

void ffmpeg_rtmp_pipe_out::write(uint8_t *data, int size) {
    fwrite(data, 1, size, _pipe_out);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000/_config.fps));
    fflush(_pipe_out);
}

ffmpeg_rtmp_pipe_out::~ffmpeg_rtmp_pipe_out(){
    pclose(_pipe_out);
}