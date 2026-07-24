#include "video/encoder/provider/basic_block/rgb_index.hpp"

struct rgb_index get_index_by_point(int frame_width, int x, int y){
    int b_pos = 3 * (y * frame_width + x);
    int g_pos = b_pos + 1, r_pos = b_pos + 2;

    struct rgb_index index = { r_pos, g_pos, b_pos };
    return index;
}

void get_index_by_point(int frame_width, point p, rgb_index &index){
    int b_pos = 3 * (p.y * frame_width + p.x);
    int g_pos = b_pos + 1, r_pos = b_pos + 2;

    index.r_index = r_pos;
    index.g_index = g_pos;
    index.b_index = b_pos;
}