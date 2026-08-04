#pragma once
#include "block_codec.hpp"
#include "config/data/palette_codec_config.hpp"

template<typename T>
class palette_codec : public block_codec
{
protected:
    T *$palette;
public:
    palette_codec(palette_codec_config<T> &config);
    int color_count();
    T* palette();
    virtual ~palette_codec();
};