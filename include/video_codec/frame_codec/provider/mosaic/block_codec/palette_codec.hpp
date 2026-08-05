#pragma once
#include "block_codec.hpp"
#include "config/data/palette_codec_config.hpp"
#include <cstring>
/* It class is contained inside .hpp completly because it is templated class */
template<typename T>
class palette_codec : public block_codec
{
protected:
    T *$palette;
public:
    palette_codec(palette_codec_config<T> &config) : block_codec(config) {
        $palette = config.palette;
    }
    T* palette(){
        int count = numbers_count();
        T *palette = new T[count];
        memcpy(palette, $palette, count * sizeof(T));
        return palette;
    }
    virtual ~palette_codec(){
        delete[] $palette;
    }
};