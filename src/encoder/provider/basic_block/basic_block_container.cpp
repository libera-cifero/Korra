#include "encoder/provider/basic_block/basic_block_container.hpp"
#include "encoder/provider/basic_block/basic_block_pointer.hpp"
#include <cstdint>

using bbc = basic_block_container;
bbc::basic_block_container(uint8_t *frame, basic_block_config config){
    _config = config;
    _begin = basic_block_pointer(frame, 0, &_config);
    _block_count = 0;
    uint32_t width_capacity = config.frame_width / config.block_size;
    uint32_t height_capacity = config.frame_height / config.block_size;
    _block_count = height_capacity * width_capacity;
    _end = basic_block_pointer(frame, _block_count - 1, &_config);
}

bbc::iterator bbc::begin() { return _begin; }

bbc::iterator bbc::end() { return _end; }

size_t bbc::count() { return _block_count; }

basic_block_pointer_proxy bbc::operator[](int index){ return *(_begin + index); }