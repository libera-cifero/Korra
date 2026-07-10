#include "encoder/provider/video_pipe/video_pipe.hpp"
video_pipe::video_pipe(video_pipe_config config){
    $config = config;
}

video_pipe_config video_pipe::config(){
    return $config;
}