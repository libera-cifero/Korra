#pragma once
#include "encoder/provider/basic_block_config.h"

struct rect{
    int x0;
    int x1;
    int y0; 
    int y1;
};

#ifdef __cplusplus
extern "C" {
#endif

struct rect get_rect(int block_index, int block_size, int width_capacity);

#ifdef __cplusplus
}
#endif