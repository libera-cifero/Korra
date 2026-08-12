#include "video_codec/frame_codec/provider/mosaic/block_codec/rgb_palette_codec.hpp"
#include "lib/color.hpp"
#include "video_codec/frame_codec/provider/mosaic/block_codec/palette_codec.hpp"
#include <cstdint>
#define UNBOX_RGB(color, r, g, b)\
char r = (char)((color & 0xff0000) >> 16), g = (char)((color & 0x00ff00) >> 8), b = (char)(color & 0x0000ff)

rgb_palette_codec::rgb_palette_codec(palette_codec_config<int> &config) : palette_codec<int>(config) { }

void rgb_palette_codec::encode(char *frame, int number, int block_index) {
    point begin, end;
    __block_index_to_area(block_index, begin, end);
    int color = __palette[number];
    UNBOX_RGB(color, red, green, blue);
    for(int y = begin.y; y < end.y; y++) {
        for(int x = begin.x; x < end.x; x++) {
            int b_pos = 3 * (y * __config.frame_width + x);
            int g_pos = b_pos + 1, r_pos = b_pos + 2;

            frame[r_pos] = red;
            frame[g_pos] = green;
            frame[b_pos] = blue;
        }
    }
}

int rgb_palette_codec::_find_nearest(char r, char g, char b) {
    int count = numbers_count();
    int min = 0;
    uint32_t min_delta = 0xffffffff;
    for(int i = 0; i < count; i++){
        int color = __palette[i];
        UNBOX_RGB(color, r1, g1, b1);
        int dr = r1 - r, dg = g1 - g, db = b1 - b;
        int delta = dr * dr + dg * dg + db * db;
        if(delta < min_delta){
            min_delta = delta;
            min = i;
        }
    }

    return min;
}

int rgb_palette_codec::decode(char *frame, int block_index) {
    uint32_t r_sum = 0, g_sum = 0, b_sum = 0;
    uint8_t *uframe = reinterpret_cast<uint8_t*>(frame);
    point begin, end;
    __block_index_to_area(block_index, begin, end);
    int c = __config.block_size * __config.block_size;
    for(int y = begin.y; y < end.y; y++) {
        for(int x = begin.x; x < end.x; x++) {
            int b_pos = 3 * (y * __config.frame_width + x);
            int g_pos = b_pos + 1, r_pos = b_pos + 2;

            r_sum += uframe[r_pos];
            g_sum += uframe[g_pos];
            b_sum += uframe[b_pos];
        }
    }
    
    char r = (char)(r_sum / c);
    char g = (char)(g_sum / c);
    char b = (char)(b_sum / c);

    return _find_nearest(r, g, b);
}

