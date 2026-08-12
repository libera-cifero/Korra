#pragma once
#include "block_codec.hpp"
#include "config/data/palette_codec_config.hpp"
/* It class is contained inside .hpp completly because it is templated class */
template<typename T>
class palette_codec : public block_codec
{
protected:
    T *__palette;
public:
    palette_codec(palette_codec_config<T> &config) : block_codec(config) {
        __palette = config.palette;
    }
    T* palette(){
        return __palette;
    }
    virtual ~palette_codec(){
        delete[] __palette;
    }
};