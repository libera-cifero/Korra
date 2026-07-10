#include "encoder/provider/video_pipe/ffmpeg_rtmp/ffmpeg_rtmp_pipe_in.hpp"
#include <cstdio>

ffmpeg_rtmp_pipe_in::ffmpeg_rtmp_pipe_in(ffmpeg_rtmp_config config){
    _config = config;
    char command[256];
    sprintf(command, "ffmpeg -i %s -f rawvideo -pix_fmt bgr24 -an pipe:1", config.rtmp_url.c_str());
    _pipe_in = popen(command, "r");
}

uint8_t *ffmpeg_rtmp_pipe_in::read(int size) {
    uint8_t *data=new uint8_t[size];
    fread(data, 1, size, _pipe_in);
    return data;
}

ffmpeg_rtmp_pipe_in::~ffmpeg_rtmp_pipe_in(){
    pclose(_pipe_in);
}