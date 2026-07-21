#pragma once
#include <cstddef>

struct video_frame {
    void *data;
    size_t size;
};