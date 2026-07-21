#include "video/encoder/provider/basic_block_settings.hpp"
#include <stdlib.h>

uint8_t *alloc_by_config(struct basic_block_settings config){
    return (uint8_t*)malloc(3 * config.frame_width * config.frame_height);
}