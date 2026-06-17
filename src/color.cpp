#include <cstdint>
#include <stdint.h>
#include <stdbool.h>
#include <algorithm>
#include <cmath>
#include "../include/color.h"

using namespace std;

typedef struct RGB
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} RGB;



uint8_t get_r(uint32_t color){
    return (uint8_t)((color & 0x00ff0000)>>16);
}

uint8_t get_g(uint32_t color){
    return (uint8_t)((color & 0x0000ff00)>>8);
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
        if(g >= b){
            return 255 * (g - b)/delta;
        }
        else{
            return 1530 - 255 * (b - g) / delta;
        }
    }
    else if(cmax == g){
        if(b >= r)
            return 510 + 255 * (b - r) / delta;
        else
            return 510 - 255 * (r - b) / delta;
    }
    else if(cmax == b){
        if(r >= g)
            return 1020 + 255 * (r - g) / delta;
        else
            return 1020 - 255 * (g - r) / delta;
    }
    else return UINT32_MAX;
}

/*
todo:
uint32_t number_to_color(uint32_t number, int color_bit_resolution);
uint32_t color_to_number(uint32_t color, int color_bit_resolution);
*/