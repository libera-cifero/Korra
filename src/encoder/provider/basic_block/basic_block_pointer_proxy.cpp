#include "encoder/provider/basic_block/basic_block_pointer_proxy.hpp"
#include "encoder/provider/basic_block/rgb_index.h"
#include "color.h"
#include <cstdint>

using bbpp = basic_block_pointer_proxy;

bbpp::basic_block_pointer_proxy(proxy_config& config)
{
    _config = config;
}

//to write
basic_block_pointer_proxy& bbpp::operator=(int data) {
    rect area = *_config.area;
    uint8_t *blocks = _config.blocks;
    uint32_t color = _config.base->codec->number_to_color(data);

    uint8_t r = get_r(color), g = get_g(color), b = get_b(color);
    int frame_width = _config.base->frame_width;
    for(int y = area.y0; y < area.y1; y++)
    {
        for(int x = area.x0; x < area.x1; x++)
        {
            rgb_index index = get_index_by_point(frame_width, x, y);
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
    int frame_width = _config.base->frame_width;

    for(int y = area.y0; y < area.y1; y++) 
    {
        for(int x = area.x0; x < area.x1; x++)
        {
            rgb_index index = get_index_by_point(frame_width,x, y);
            r_sum += blocks[index.r_index];
            g_sum += blocks[index.g_index];
            b_sum += blocks[index.b_index];
        }
    }

    c = ((r_sum / c) << 16) | ((g_sum / c) << 8) | (b_sum / c);
    return _config.base->codec->color_to_number(c);
}