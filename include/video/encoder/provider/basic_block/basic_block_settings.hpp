#pragma once
#include "video/encoder/provider/basic_block/color_codec/color_codec.hpp"
#include "config/data/video_config.hpp"
#include <stdint.h>

//basic block container config
struct basic_block_settings : public video_config {
    uint32_t block_size;
    color_codec *codec;
};

#ifdef __cplusplus
extern "C" {
#endif
uint8_t *alloc_by_config(struct basic_block_settings config);
#ifdef __cplusplus
}
#endif