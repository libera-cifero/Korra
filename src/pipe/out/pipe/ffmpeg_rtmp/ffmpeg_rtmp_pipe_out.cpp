#include "pipe/out/pipe/ffmpeg_rtmp/ffmpeg_rtmp_pipe_out.hpp"
#include <format>

ffmpeg_rtmp_pipe_out::ffmpeg_rtmp_pipe_out(ffmpeg_rtmp_config &config){
    __config = config;
    __cmd = std::format(
        "ffmpeg -fflags nobuffer -flags low_delay -f rawvideo -pix_fmt bgr24 -s {}x{} -r {} -i pipe:0 "
        "-c:v libx264 -preset ultrafast -tune zerolatency "
        "-pix_fmt yuv420p -f flv {}",
        __config.frame_width, __config.frame_height, __config.fps, __config.rtmp_url.c_str()
    );
}

std::string ffmpeg_rtmp_pipe_out::url(){
    return __config.rtmp_url;
}

int ffmpeg_rtmp_pipe_out::__frame_size() { return __config.frame_height * __config.frame_width  * 3; }