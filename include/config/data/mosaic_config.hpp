#pragma once
#include "config/data/video_config.hpp"
struct mosaic_config : public video_config {
    uint32_t block_size;
};