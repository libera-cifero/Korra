#include "encoder/provider/basic_block/rect.h"

struct rect get_rect(int block_index, int block_size, int width_capacity){
    int index_y = block_index / width_capacity, index_x = block_index % width_capacity;
    int y0 = index_y * block_size, x0 = index_x * block_size;
    int y1 = y0 + block_size, x1 = x0 + block_size;
    
    struct rect area = {
        .x0 = x0,
        .x1 = x1,
        .y0 = y0,
        .y1 = y1
    };
    
    return area;
}