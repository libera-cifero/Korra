#pragma once
struct rgb_index{
    int r_index;
    int g_index;
    int b_index;
};

#ifdef __cplusplus
extern "C" {
#endif

struct rgb_index get_index_by_point(int frame_width, int x, int y);

#ifdef __cplusplus
}
#endif