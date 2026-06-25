#include "encoder/provider/basic_block/basic_block_container.hpp"
#include "encoder/provider/basic_block/basic_block_pointer.hpp"

using bbc = basic_block_container;
bbc::basic_block_container(uint8_t *frame, basic_block_config config){
    _config = config;
    _begin = basic_block_pointer(frame, 0, &_config);
    
    _end = basic_block_pointer(frame, -1, &_config);
}