#pragma once
#include "video_codec/frame_codec/provider/provider.hpp"
struct area_size {
    int width, height;
};

class size_generator {
public:
    virtual bool can_generate(provider *) = 0;
    virtual area_size random_size(provider *, area_size) = 0;
    virtual ~size_generator() {};
};