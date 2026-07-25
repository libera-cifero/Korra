#pragma once
#include "point.hpp"
struct rgb_index{
    int r_index;
    int g_index;
    int b_index;
};
struct rgb_index get_index_by_point(int frame_width, int x, int y);
void get_index_by_point(int frame_width, point p, rgb_index &index);