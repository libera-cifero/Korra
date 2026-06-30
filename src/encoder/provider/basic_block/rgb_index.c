#include "encoder/provider/basic_block/rgb_index.h"

struct rgb_index get_index_by_point(int frame_width, int x, int y){
    int r_pos = 3 * (y * frame_width + x);
    int g_pos = r_pos + 1, b_pos = r_pos + 2;

    struct rgb_index index = { r_pos, g_pos, b_pos };
    return index;
}