#include "basic_block_container.hpp"
#include <cstdint>

struct rect{
    int x0;
    int x1;
    int y0; 
    int y1;
};

struct rgb_index{
    int r_index;
    int g_index;
    int b_index;
};

class basic_block_ref {
private:
    uint8_t *_blocks;
    uint32_t _block_index;

    bbc_config *_config;
    uint32_t _width_capacity;

    rect _rect;
    
    rect _get_rect();
    rgb_index _get_index_by_point(int x, int y);
public:
    basic_block_ref(uint8_t *blocks, uint32_t block_index, bbc_config *config);

    int operator*();
    basic_block_ref& operator=(int data);
};