#pragma once
#include <stdint.h>

//basic block container config
struct basic_block_config{
    uint32_t bits_per_block;
    uint32_t block_size;
    uint32_t frame_width;
    uint32_t frame_height;
};

#ifdef __cplusplus
extern "C" {
#endif
uint8_t *alloc_by_config(struct basic_block_config config);
#ifdef __cplusplus
}
#endif