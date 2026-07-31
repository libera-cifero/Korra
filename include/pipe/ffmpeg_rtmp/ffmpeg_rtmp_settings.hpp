#pragma once
#include "config/data/video_config.hpp"
#include <string>

struct ffmpeg_rtmp_settings : video_config {
    std::string rtmp_url;
};