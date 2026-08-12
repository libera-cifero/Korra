#include "video_codec/frame_codec/provider/mosaic/block_codec/block_codec.hpp"
#include "config/data/block_codec_config.hpp"

block_codec::block_codec(block_codec_config &config){
    __config = config;
}

int block_codec::bits_per_number(){
    return __config.bits_per_number;
}

int block_codec::numbers_count(){
    return 1 << __config.bits_per_number;
}

int block_codec::block_size(){
    return __config.block_size;
}

block_codec_config block_codec::config(){
    return __config;
}

void block_codec::__block_index_to_area(int block_index, point &begin, point &end) {
    int block_size = __config.block_size;
    int width_capacity = __config.frame_width / block_size;
    int index_y = block_index / width_capacity, index_x = block_index % width_capacity;
    int y = index_y * block_size, x = index_x * block_size;

    begin.x = x;
    begin.y = y;

    end.x = x + __config.block_size;
    end.y = y + __config.block_size;
}

block_codec::~block_codec() { }