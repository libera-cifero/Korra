#include <cstdint>
#include <cstddef>

uint32_t get_width_capacity(int frame_width, int block_size);
uint32_t get_height_capacity(int frame_height, int block_size);
uint32_t get_block_count(int frame_width, int frame_height, int block_size);
uint32_t get_basic_block_frame_payload_size(int bits_per_block, int block_count);
size_t get_block_count(size_t byte_size, size_t bits_per_block);