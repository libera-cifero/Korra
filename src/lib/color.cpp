#include <cstdint>
#include <cstdio>
#include <stdint.h>
#include <stdbool.h>
#include <algorithm>
#include <cmath>
#include "lib/color.hpp"
#include "lib/status_error.hpp"

using namespace std;

uint8_t get_r(uint32_t color){
    return (uint8_t)((color & 0xff0000)>>16);
}

uint8_t get_g(uint32_t color){
    return (uint8_t)((color & 0x00ff00)>>8);
}

uint8_t get_b(uint32_t color){
    return (uint8_t)(color & 0x0000ff);
}

string rgb_to_hex(int rgb){
    int r = (rgb >> 16) & 0xFF;
    int g = (rgb >> 8) & 0xFF;
    int b = rgb & 0xFF;

    char buf[8];
    sprintf(buf, "#%02x%02x%02x", r,g,b);
    return string(buf); 
}