#include <stdint.h>
#include <stdbool.h>
#include "../include/color.h"

//0, 255, 511, 767, 1023, 1279, 1535
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

uint32_t get_index_by_color(uint32_t color)
{
    if(color > 0xffffff){
        return UINT32_MAX;
    }
    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8)  & 0xFF;
    uint8_t b =  color        & 0xFF;

    if (r == 255 && b == 0)
        return g;                    // sector 0
    else if (g == 255 && b == 0)
        return 255 + (255 - r);      // sector 1
    else if (g == 255 && r == 0)
        return 510 + b;              // sector 2
    else if (b == 255 && r == 0)
        return 765 + (255 - g);      // sector 3
    else if (b == 255 && g == 0)
        return 1020 + r;             // sector 4
    else if (r == 255 && g == 0)
        return 1275 + (255 - b);     // sector 5
    else return UINT32_MAX;
}