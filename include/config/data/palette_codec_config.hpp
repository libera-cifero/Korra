#pragma once
#include "block_codec_config.hpp"

template<typename T>
struct palette_codec_config : public block_codec_config {
    T *palette;
};

//template struct palette_codec_config<int>;