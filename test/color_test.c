#include "../include/color.h"
#include <stdio.h>

int main(){
    uint32_t t = 0;
    
    for(uint32_t i = 0; i < 19; i++){
        uint32_t color = get_color_by_index(i);
        uint32_t r = get_r(color), g = get_g(color), b = get_b(color);
        printf("%u %06x rgb(%u, %u, %u)\n", i, color, r, g, b);
    }

    return 0;
}