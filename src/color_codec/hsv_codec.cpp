#include "color_codec/hsv_codec.hpp"
#include "color_codec/color_codec.hpp"
#include "color.h"
#include <cstdio>
#include <algorithm>
#include <cstring>

hsv_codec::hsv_codec(int bits_per_number) : color_codec(bits_per_number) { }

static int32_t div_round(int32_t a, int32_t b)
{
    if (a >= 0)
        return (a + b / 2) / b;
    else
        return (a - b / 2) / b;
}

int hsv_codec::_get_index_by_color(int color){
    if(color > 0xffffff){
        return UINT32_MAX;
    }
    uint32_t r = get_r(color), g = get_g(color), b = get_b(color);
    uint32_t cmax = std::max({r,g,b}), cmin = std::min({r,g,b});
    uint32_t delta = cmax - cmin;
    if(delta == 0) {
        return 0;
    }
    else if(cmax == r){
        if(g >= b)
            return div_round(255 * (g - b), delta);
        else
            return 1530 - div_round(255 * (b - g), delta);
    }
    else if(cmax == g){
        if(b >= r)
            return 510 + div_round(255 * (b - r), delta);
        else
            return 510 - div_round(255 * (r - b), delta);
    }
    else if(cmax == b){
        if(r >= g)
            return 1020 + div_round(255 * (r - g), delta);
        else
            return 1020 - div_round(255 * (g - r), delta);
    }
    else return UINT32_MAX;
}

int hsv_codec::_get_color_by_index(int color_index){
    uint32_t color_byte = color_index % 255;
    uint32_t sub = 255 - color_byte;
    if(color_index < 255){
        return 0xff0000 | (color_byte << 8);
    }
    else if(color_index < 510){
        return 0x00ff00 | (sub << 16);
    }
    else if(color_index < 765) {
        return 0x00ff00 | color_byte;
    }
    else if(color_index < 1020){
        return 0x0000ff | (sub << 8);
    }
    else if(color_index < 1275){
        return 0x0000ff | (color_byte << 16);
    }
    else if(color_index < 1530){
        return 0xff0000 | sub;
    }
    else return UINT32_MAX;
}

int hsv_codec::_number_to_index(int number, int color_bit_resolution) { 
    return div_round(number * COLOR_palette, (1<<color_bit_resolution));
}

int hsv_codec::_index_to_number(int number, int color_bit_resolution) {
    return div_round(number * COLOR_palette, (1<<color_bit_resolution));
}

int hsv_codec::color_to_number(int color) {
    color_codec::color_to_number(color);
    int available_numbers = 1 << bits_per_number();
    int color_index = _get_index_by_color(color);
    int number_predicted = color_index * available_numbers / COLOR_palette, color_index_min, color_index_max;
    if(number_predicted == available_numbers - 1){
        color_index_min = _number_to_index(number_predicted, bits_per_number());
        color_index_max = 0;
    }
    else{
        color_index_min = _number_to_index(number_predicted, bits_per_number());
        color_index_max = _number_to_index(number_predicted + 1, bits_per_number());
    }

    bool is_min = abs(color_index - color_index_min) <= abs(color_index_max - color_index);
    return (uint16_t)(is_min ? number_predicted : ++number_predicted);
}

int hsv_codec::number_to_color(int number) {
    color_codec::number_to_color(number);
    return get_color_by_index(number_to_index(number, bits_per_number()));
}
