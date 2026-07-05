#include "color_codec/palette_codec.hpp"
#include "color_codec/color_codec.hpp"
#include <cmath>

palette_codec::palette_codec(int *palette, int bits_per_number) : color_codec(bits_per_number){
    _palette = palette;
}

int palette_codec::color_to_number(int color) {
    color_codec::color_to_number(color);
    float min_distance = MAXFLOAT;
    int min_index = -1;
    for(int i = 0; i < max_number(); i++){
        float dist = $get_distance(color, _palette[i]);
        if(dist < min_distance)
        {
            min_distance = dist;
            min_index = i;
        }
    }

    return min_index;
}

int palette_codec::number_to_color(int number) {
    color_codec::number_to_color(number);
    return _palette[number];
}

palette_codec::~palette_codec(){
    delete[] _palette;
}