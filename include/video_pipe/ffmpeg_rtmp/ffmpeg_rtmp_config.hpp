#include "../video_pipe_config.hpp"
#include <string>

struct ffmpeg_rtmp_config : video_pipe_config {
    std::string rtmp_url;
};