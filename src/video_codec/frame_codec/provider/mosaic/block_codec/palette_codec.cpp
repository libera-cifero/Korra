#include "video_codec/frame_codec/provider/mosaic/block_codec/palette_codec.hpp"
#include <cstring>

template<typename T>
palette_codec<T>::palette_codec(palette_codec_config<T> &config) : block_codec(config) {
    int count = color_count();
    $palette = new T[count];
    memcpy($palette, config.palette, count * sizeof(T));
}

template<typename T>
T* palette_codec<T>::palette(){
    int count = color_count();
    T *palette = new T[count];
    memcpy(palette, $palette, count * sizeof(T));
    return palette;
}

template<typename T>
int palette_codec<T>::color_count(){
    return 1 << $config.bits_per_number;
}

template<typename T>
palette_codec<T>::~palette_codec(){
    delete[] $palette;
}
template class palette_codec<int>;