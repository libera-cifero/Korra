#include "encoder/provider/basic_block/basic_block_pointer_proxy.hpp"
#include "color.h"
#include <cstdint>

using bbpp = basic_block_pointer_proxy;

bbpp::basic_block_pointer_proxy(proxy_config& config)
{
    _config = config;
}

rgb_index bbpp::_get_index_by_point(int x, int y) const {
    int r_pos = 3 * (y * _config.base->frame_width + x);
    int g_pos = r_pos + 1, b_pos = r_pos + 2;

    return { .r_index = r_pos, .g_index = g_pos, .b_index = b_pos };
}

//to write
basic_block_pointer_proxy& bbpp::operator=(int data) {
    rect area = *_config.area;
    uint8_t *blocks = _config.blocks;
    uint32_t color = number_to_color(data, _config.base->bits_per_block);

    uint8_t r = get_r(color), g = get_g(color), b = get_b(color);

    for(int y = area.y0; y < area.y1; y++)
    {
        for(int x = area.x0; x < area.x1; x++)
        {
            rgb_index index = _get_index_by_point(x, y);
            blocks[index.r_index] = r;
            blocks[index.g_index] = g;
            blocks[index.b_index] = b;
        }
    }

    return *this;
}

//to read
bbpp::operator int() const {
    rect area = *_config.area;
    uint8_t *blocks = _config.blocks;
    uint32_t r_sum = 0, g_sum = 0, b_sum = 0, c = (area.y1 - area.y0) * (area.x1 - area.x0);
    
    for(int y = area.y0; y < area.y1; y++) 
    {
        for(int x = area.x0; x < area.x1; x++)
        {
            rgb_index index = _get_index_by_point(x, y);
            r_sum += blocks[index.r_index];
            g_sum += blocks[index.g_index];
            b_sum += blocks[index.b_index];
        }
    }

    c = ((r_sum / c) << 16) | ((g_sum / c) << 8) | (b_sum / c);
    return color_to_number(c, _config.base->bits_per_block);
}