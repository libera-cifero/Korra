#include "color_codec/rgb_palette_codec.hpp"
#include "color.hpp"

rgb_palette_codec::rgb_palette_codec(int *palette, int bits_per_number):palette_codec(palette, bits_per_number) { }
float rgb_palette_codec::$get_distance(int color0, int color1) {
    int r0 = get_r(color0), g0 = get_g(color0), b0 = get_b(color0);
    int r1 = get_r(color1), g1 = get_g(color1), b1 = get_b(color1);

    int dr = r1 - r0, dg = g1 - g0, db = b1 - b0;

    return dr * dr + dg * dg + db * db;
}