#include "video_codec/frame_codec/provider/mosaic/color_codec/block_codec.hpp"

block_codec::block_codec(int bits_per_number, int block_size, char *frame){
    _bits_per_number = bits_per_number;
    _block_size = block_size;
    _frame = frame;
}

int block_codec::bits_per_number(){
    return _bits_per_number;
}