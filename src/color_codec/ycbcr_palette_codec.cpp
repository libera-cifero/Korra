#include "color_codec/ycbcr_palette_codec.hpp"
#include "color_codec/palette_codec.hpp"
#include <cstdlib>

ycbcr_palette_codec::ycbcr_palette_codec(int *palette, int bits_per_number):palette_codec(palette, bits_per_number) { }
float ycbcr_palette_codec::$get_distance(int color0, int color1) {
    return std::abs(color1 - color0);
}