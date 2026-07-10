#include "encoder/provider/video_pipe/ffmpeg_rtmp/ffmpeg_rtmp_pipe_out.hpp"

ffmpeg_rtmp_pipe_out::ffmpeg_rtmp_pipe_out(ffmpeg_rtmp_config config){
    _config = config;
}

void ffmpeg_rtmp_pipe_out::write(uint8_t *data, int size) {
    
}

ffmpeg_rtmp_pipe_out::~ffmpeg_rtmp_pipe_out(){
    
}