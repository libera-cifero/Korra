#include "video_encoder/frame_encoder/provider/mosaic/mosaic_math.hpp"
#include <cstdint>
#include <cstddef>

uint32_t get_width_capacity(int frame_width, int block_size) { return frame_width / block_size; }

uint32_t get_height_capacity(int frame_height, int block_size) { return frame_height / block_size; }

uint32_t get_block_count(int frame_width, int frame_height, int block_size){
    uint32_t width_capacity = get_width_capacity(frame_width, block_size);
    uint32_t height_capacity = get_height_capacity(frame_height, block_size);
    return height_capacity * width_capacity;
}

uint32_t get_mosaic_frame_payload_size(int bits_per_block, int block_count) {
    return bits_per_block * block_count / 8;
}

size_t get_block_count(size_t byte_size, size_t bits_per_block) {
    size_t bit_size = 8 * byte_size;
    size_t count = bit_size / bits_per_block;
    if(bit_size % bits_per_block > 0) count+=1;
    return count;
}
