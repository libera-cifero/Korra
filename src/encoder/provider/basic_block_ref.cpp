#include "encoder/provider/basic_block_ref.hpp"
#include "encoder/provider/basic_block_container.hpp"
#include "color.h"

rect basic_block_ref::_get_rect() {
    int block_size = _config->block_size;
    int index_y = _block_index / _width_capacity, index_x = _block_index % _width_capacity;
    int y0 = index_y * block_size, x0 = index_x * block_size;
    int y1 = y0 + block_size, x1 = x0 + block_size;
    
    return {
        .x0 = x0,
        .x1 = x1,
        .y0 = y0,
        .y1 = y1
    };
}

basic_block_ref::basic_block_ref(uint8_t *blocks, uint32_t block_index, bbc_config *config){
    _blocks = blocks;
    _block_index = block_index;
    _config = config;
    _width_capacity = config->frame_width / config->block_size;
    _rect = _get_rect();
}

rgb_index basic_block_ref::_get_index_by_point(int x, int y){
    int r_pos = 3 * (y * _config->frame_width + x);
    int g_pos = r_pos + 1, b_pos = r_pos + 2;

    return { .r_index = r_pos, .g_index = g_pos, .b_index = b_pos };
}

basic_block_ref& basic_block_ref::operator=(int data){
    uint32_t color = number_to_color(data, _config->bits_per_block);

    uint8_t r = get_r(color), g = get_g(color), b = get_b(color);

    for(int y = _rect.y0; y < _rect.y1; y++) 
    {
        for(int x = _rect.x0; x < _rect.x1; x++)
        {
            rgb_index index = _get_index_by_point(x, y);
            _blocks[index.r_index] = r;
            _blocks[index.g_index] = g;
            _blocks[index.b_index] = b;
        }
    }

    return *this;
}

int basic_block_ref::operator*(){
    uint32_t r_sum = 0, g_sum = 0, b_sum = 0, c = (_rect.y1 - _rect.y0) * (_rect.x1 - _rect.x0);
    
    for(int y = _rect.y0; y < _rect.y1; y++) 
    {
        for(int x = _rect.x0; x < _rect.x1; x++)
        {
            rgb_index index = _get_index_by_point(x, y);
            r_sum += _blocks[index.r_index];
            g_sum += _blocks[index.g_index];
            b_sum += _blocks[index.b_index];
        }
    }

    c = ((r_sum / c) << 16) | ((g_sum / c) << 8) | (b_sum / c);
    return color_to_number(c, _config->bits_per_block);
}