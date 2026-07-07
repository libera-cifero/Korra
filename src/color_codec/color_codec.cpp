#include "color_codec/color_codec.hpp"
#include "status_error.hpp"

color_codec::color_codec(int bits_per_number) {
    _bits_per_number = bits_per_number;
}

int color_codec::bits_per_number(){ return _bits_per_number; }
int color_codec::color_count(){ return (1<<_bits_per_number); }

int color_codec::number_to_color(int number){
    if(number > color_count() - 1){
        char error_buff[256];
        sprintf(error_buff,"color_bit_resolution equals %d must be less %d!", _bits_per_number, color_count());
        throw status_error(error_buff, 1);
    }

    return -1;
}

int color_codec::color_to_number(int color){
    if(color > 0xffffff){
        char buff[256];
        sprintf(buff, "Invalid color %x", color);
        throw status_error(buff, 1);
    }

    return -1;
}