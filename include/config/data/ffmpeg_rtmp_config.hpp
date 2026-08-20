#include "video_config.hpp"
#include <string>

struct ffmpeg_rtmp_config : video_config {
    std::string rtmp_url;
};