#include "pipe/out/rutube_rtmp_out_manager.hpp"
#include "config/data/video_config.hpp"
#include "pipe/out/pipe/video_pipe_out.hpp"

rutube_rtmp_out_manager::rutube_rtmp_out_manager(video_config &config){
    _config = config;
}

video_pipe_out *rutube_rtmp_out_manager::build(){
    
}

void rutube_rtmp_out_manager::destroy(video_pipe_out *){
    
}