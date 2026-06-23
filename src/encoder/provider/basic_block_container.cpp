#include "encoder/provider/basic_block_container.hpp"
#include "color.h"
#include "status_error.hpp"
#include <cstdint>
#include <cstdio>

using bbc = basic_block_container;

bbc::basic_block_container(uint8_t *frame, bbc_config config) {
    _frame = frame;
    _bits_per_block = config.bits_per_block;
    _block_size = config.block_size;
    _frame_width = config.frame_width;
    _frame_height = config.frame_height;

    _block_count = 0;
    _width_capacity = _frame_width / _block_size;
    _height_capacity = _frame_height / _block_size;
    _block_capacity = _height_capacity * _width_capacity;
}

uint32_t bbc::count() { return _block_count; }

uint32_t bbc::capacity() { return _block_capacity; }

uint32_t bbc::bits_per_block(){ return _bits_per_block; }

bbc::rect bbc::_get_rect_by_index(uint32_t block_index)
{
    int index_y = block_index / _width_capacity, index_x = block_index % _width_capacity;
    int y0 = index_y * _block_size, x0 = index_x * _block_size;
    int y1 = y0 + _block_size, x1 = x0 + _block_size;
    
    return {
        .x0 = x0,
        .x1 = x1,
        .y0 = y0,
        .y1 = y1
    };
}

bbc::rgb_index bbc::_get_index_by_point(int x, int y){
    int r_pos = 3 * (y * _frame_width + x);
    int g_pos = r_pos + 1, b_pos = r_pos + 2;

    return { .r_index = r_pos, .g_index = g_pos, .b_index = b_pos };
}


uint32_t bbc::operator[](int block_index) {
    rect block = _get_rect_by_index(block_index);

    uint32_t r_sum = 0, g_sum = 0, b_sum = 0, c = (block.y1 - block.y0) * (block.x1 - block.x0);
    
    for(int y = block.y0; y < block.y1; y++) 
    {
        for(int x = block.x0; x < block.x1; x++)
        {
            rgb_index index = _get_index_by_point(x, y);
            r_sum += _frame[index.r_index];
            g_sum += _frame[index.g_index];
            b_sum += _frame[index.b_index];
        }
    }

    c = ((r_sum / c) << 16) | ((g_sum / c) << 8) | (b_sum / c);
    return color_to_number(c, _bits_per_block);
}

void bbc::append_block(uint32_t data) {
    if(_block_count >= _block_capacity){
        throw status_error("container is filled already!", 1);
    }

    uint32_t color = number_to_color(data, _bits_per_block);
    rect block = _get_rect_by_index(_block_count);

    uint8_t r = get_r(color), g = get_g(color), b = get_b(color);

    for(int y = block.y0; y < block.y1; y++) 
    {
        for(int x = block.x0; x < block.x1; x++)
        {
            rgb_index index = _get_index_by_point(x, y);
            _frame[index.r_index] = r;
            _frame[index.g_index] = g;
            _frame[index.b_index] = b;
        }
    }

    _block_count++;
}