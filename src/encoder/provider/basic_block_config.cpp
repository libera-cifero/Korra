#include "encoder/provider/basic_block_config.hpp"
#include <stdlib.h>

uint8_t *alloc_by_config(struct basic_block_config config){
    return (uint8_t*)malloc(3 * config.frame_width * config.frame_height);
}