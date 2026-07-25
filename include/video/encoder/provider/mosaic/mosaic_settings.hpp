#pragma once
#include "video/encoder/provider/mosaic/color_codec/color_codec.hpp"
#include "config/data/video_config.hpp"
#include <stdint.h>

//basic block container config
struct mosaic_settings : public video_config {
    uint32_t block_size;
    color_codec *codec;
};