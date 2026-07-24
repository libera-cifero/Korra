#pragma once

typedef struct rect{
    int x0;
    int x1;
    int y0; 
    int y1;
} rect;

struct rect get_rect(int block_index, int block_size, int width_capacity);