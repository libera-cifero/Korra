#include <stdint.h>
#include <stdbool.h>
#include "../include/color.h"

static const uint32_t base_colors[6] = { 0xff0000, 0xffff00, 0x00ff00, 0x00ffff, 0x0000ff, 0xff00ff };
static const uint8_t color_bytes[3] = {2,1,3 };

uint8_t get_r(uint32_t color){
    return (uint8_t)((color & 0x00ff0000)>>16);
}

uint8_t get_g(uint32_t color){
    return (uint8_t)((color & 0x0000ff00)>>8);
}

uint8_t get_b(uint32_t color){
    return (uint8_t)(color & 0x000000ff);
}

uint32_t get_color_by_index(uint32_t color_index) {
    uint32_t sector = color_index / 256;
    bool is_decrease = sector % 2;
    uint8_t byte_position = color_bytes[sector % 3];
    uint8_t bit_position = 8 * byte_position;
    uint32_t color = base_colors[sector], color_shift = 24 - bit_position;

    uint32_t color_mask = 0x000000ff << color_shift;

    uint32_t color_byte = (color & color_mask) >> color_shift;
    uint32_t delta_byte = color_index % 256;
    
    if(is_decrease)
        color_byte -= delta_byte;
    else
        color_byte += delta_byte;
    
    color &= ~color_mask;

    color |= color_byte << color_shift;

    return color;
}

