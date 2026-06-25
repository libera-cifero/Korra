#include "encoder/provider/basic_block/basic_block_pointer_proxy.hpp"
#include "encoder/provider/basic_block/basic_block_pointer.hpp"
#include "encoder/provider/basic_block_config.h"
#include "color.h"

using bbp = basic_block_pointer;

rect bbp::_get_rect() {
    int block_size = _config->block_size;
    int index_y = _block_index / _width_capacity, index_x = _block_index % _width_capacity;
    int y0 = index_y * block_size, x0 = index_x * block_size;
    int y1 = y0 + block_size, x1 = x0 + block_size;
    
    return {
        .x0 = x0,
        .x1 = x1,
        .y0 = y0,
        .y1 = y1
    };
}

bbp::basic_block_pointer(uint8_t *blocks, uint32_t block_index, basic_block_config *config){
    _blocks = blocks;
    _block_index = block_index;
    _config = config;
    _width_capacity = config->frame_width / config->block_size;
    _rect = _get_rect();
}

basic_block_pointer_proxy& basic_block_pointer::operator*(){
    proxy_config config={.block_index=_block_index};
    auto proxy = new basic_block_pointer_proxy(config);
    return *proxy;
}