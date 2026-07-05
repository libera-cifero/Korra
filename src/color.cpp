#include <cstdint>
#include <cstdio>
#include <stdint.h>
#include <stdbool.h>
#include <algorithm>
#include <cmath>
#include "color.h"
#include "status_error.hpp"

using namespace std;

uint8_t get_r(uint32_t color){
    return (uint8_t)((color & 0xff0000)>>16);
}

uint8_t get_g(uint32_t color){
    return (uint8_t)((color & 0x00ff00)>>8);
}

uint8_t get_b(uint32_t color){
    return (uint8_t)(color & 0x000000ff);
}

uint32_t get_color_by_index(uint32_t color_index) 
{
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

static int32_t div_round(int32_t a, int32_t b)
{
    if (a >= 0)
        return (a + b / 2) / b;
    else
        return (a - b / 2) / b;
}

uint32_t get_index_by_color(uint32_t color)
{
    if(color > 0xffffff){
        return UINT32_MAX;
    }
    uint32_t r = get_r(color), g = get_g(color), b = get_b(color);
    uint32_t cmax = max({r,g,b}), cmin = min({r,g,b});
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

uint32_t index_to_number_base(uint32_t index, int color_bit_resolution){
    return index * (1<<color_bit_resolution) / COLOR_palette;
}

uint32_t number_to_index(uint32_t number, int color_bit_resolution){
    return div_round(number * COLOR_palette, (1<<color_bit_resolution));
}

uint32_t number_to_color(uint16_t number, int color_bit_resolution){
    char error_buff[256];
    if(color_bit_resolution > 10){
        sprintf(error_buff,"color_bit_resolution equals %d must be less or equal 10!", color_bit_resolution);
        throw status_error(error_buff, 1);
    }
    uint32_t available_numbers = 1 << color_bit_resolution;
    if(number > available_numbers - 1){
        sprintf(error_buff, "number %u is too large for color_bit_resolution %d", number, color_bit_resolution);
        throw status_error(error_buff,2);
    }
    return get_color_by_index(number_to_index(number, color_bit_resolution));
}

uint16_t color_to_number(uint32_t color, int color_bit_resolution) {
    if(color > 0xffffff){
        char buff[256];
        sprintf(buff, "Invalid color %x", color);
        throw status_error(buff, 1);
    }
    int available_numbers = 1 << color_bit_resolution;
    int color_index = get_index_by_color(color);
    int number_predicted = color_index * available_numbers / COLOR_palette, color_index_min, color_index_max;
    if(number_predicted == available_numbers - 1){
        color_index_min = number_to_index(number_predicted, color_bit_resolution);
        color_index_max = 0;
    }
    else{
        color_index_min = number_to_index(number_predicted, color_bit_resolution);
        color_index_max = number_to_index(number_predicted + 1, color_bit_resolution);
    }

    bool is_min = abs(color_index - color_index_min) <= abs(color_index_max - color_index);
    return (uint16_t)(is_min ? number_predicted : ++number_predicted);
}