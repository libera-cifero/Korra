#include "video_codec/frame_codec/provider/mosaic/block_codec/block_codec.hpp"
#include "config/data/block_codec_config.hpp"

block_codec::block_codec(block_codec_config &config){
    $config = config;
}

int block_codec::bits_per_number(){
    return $config.bits_per_number;
}

int block_codec::numbers_count(){
    return 1 << $config.bits_per_number;
}

int block_codec::block_size(){
    return $config.block_size;
}

block_codec_config block_codec::config(){
    return $config;
}

void block_codec::$block_index_to_area(int block_index, point &begin, point &end) {
    int block_size = $config.block_size;
    int width_capacity = $config.frame_width / block_size;
    int index_y = block_index / width_capacity, index_x = block_index % width_capacity;
    int y = index_y * block_size, x = index_x * block_size;

    begin.x = x;
    begin.y = y;

    end.x = x + $config.block_size;
    end.y = y + $config.block_size;
}

block_codec::~block_codec() { }