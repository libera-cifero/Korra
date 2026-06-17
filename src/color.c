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
    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8)  & 0xFF;
    uint8_t b =  color        & 0xFF;

    if (r == 255 && b == 0)
        return g;                    // sector 0

    if (g == 255 && b == 0)
        return 256 + (255 - r);      // sector 1

    if (g == 255 && r == 0)
        return 512 + b;              // sector 2

    if (b == 255 && r == 0)
        return 768 + (255 - g);      // sector 3

    if (b == 255 && g == 0)
        return 1024 + r;             // sector 4

    if (r == 255 && g == 0)
        return 1280 + (255 - b);     // sector 5

    return UINT32_MAX;
}

/*uint32_t downscale_channel(uint8_t channel, uint32_t channel_capacity){
    return 256 / channel_capacity * channel - 1;
}

uint32_t downscale(uint32_t color, uint8_t bit_resolution){
    if(bit_resolution > 24){
        return 0xffffffff;
    }
    else if(bit_resolution % 3 != 0){
        return 0xfffffffe;
    }
    uint8_t channel_capacity = 1 << (bit_resolution / 3 - 1);
    uint32_t r0 = (uint32_t)get_r(color), g0 = (uint32_t)get_g(color), b0 = (uint32_t)get_b(color);
    uint32_t r1 = downscale_channel(r0, channel_capacity), 
        g1 = downscale_channel(g0, channel_capacity),
        b1 = downscale_channel(b0, channel_capacity);
    
}*/