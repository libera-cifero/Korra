#include "pipe/in/pipe/ffmpeg_rtmp_pipe_in.hpp"
#include <cstdio>
#include <format>
#include <fcntl.h>
#include <sys/cdefs.h>

ffmpeg_rtmp_pipe_in::ffmpeg_rtmp_pipe_in(ffmpeg_rtmp_config &config){
    _config = config;
    __cmd = std::format(
        "ffmpeg -loglevel error -i {} -rtmp_buffer 60000 -an -f rawvideo -pix_fmt bgr24 -vf scale={}x{} pipe:1",
        config.rtmp_url.c_str(),
        config.frame_width,
        config.frame_height
    );
    _pipe_in = popen(__cmd.c_str(), "r");
}

std::string ffmpeg_rtmp_pipe_in::url(){
    return _config.rtmp_url;
}
int ffmpeg_rtmp_pipe_in::_frame_size() { return _config.frame_height * _config.frame_width  * 3; }

char *ffmpeg_rtmp_pipe_in::read(int size) {
    auto data=new char[size];
    fread(data, 1, size, _pipe_in);
    
    return data;
}

ffmpeg_rtmp_pipe_in::~ffmpeg_rtmp_pipe_in(){
    pclose(_pipe_in);
}