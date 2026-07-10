#include "encoder/provider/video_pipe/ffmpeg_rtmp/ffmpeg_rtmp_pipe_in.hpp"

ffmpeg_rtmp_pipe_in::ffmpeg_rtmp_pipe_in(ffmpeg_rtmp_config config){
    _config = config;
}

uint8_t *ffmpeg_rtmp_pipe_in::read(int size) {

}

ffmpeg_rtmp_pipe_in::~ffmpeg_rtmp_pipe_in(){

}