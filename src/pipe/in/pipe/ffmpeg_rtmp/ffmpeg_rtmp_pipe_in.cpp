#include "pipe/in/pipe/ffmpeg_rtmp/ffmpeg_rtmp_pipe_in.hpp"
#include <cstring>
#include <format>
#include <fcntl.h>
#include <sys/cdefs.h>

ffmpeg_rtmp_pipe_in::ffmpeg_rtmp_pipe_in(ffmpeg_rtmp_config &config){
    __pipe = nullptr;
    __config = config;
    __cmd = std::format(
        "ffmpeg -loglevel error -i {} -rtmp_buffer 60000 -an -f rawvideo -pix_fmt bgr24 -vf scale={}x{} pipe:1",
        config.rtmp_url.c_str(),
        config.frame_width,
        config.frame_height
    );
    __frame_size = __config.frame_height * __config.frame_width  * 3;
    __buffer = new char[__frame_size];
    __bytes_readed = 0;
}

std::string ffmpeg_rtmp_pipe_in::url(){
    return __config.rtmp_url;
}

char *ffmpeg_rtmp_pipe_in::read(int size) {
    int recv_count = __read_async();
    if(recv_count <= 0) return nullptr;
    __bytes_readed += recv_count;
    if(__bytes_readed >= __frame_size){
        __bytes_readed = 0;
        char *frame = new char[__frame_size];
        memcpy(frame, __buffer, __frame_size);
        return frame;
    }
    return nullptr;
}

ffmpeg_rtmp_pipe_in::~ffmpeg_rtmp_pipe_in() {
    delete [] __buffer;
}