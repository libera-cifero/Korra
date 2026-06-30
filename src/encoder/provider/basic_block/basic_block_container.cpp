#include "encoder/provider/basic_block/basic_block_container.hpp"
#include "encoder/provider/basic_block/basic_block_pointer.hpp"
#include "encoder/provider/basic_block/basic_block_pointer_proxy.hpp"
#include "encoder/provider/basic_block_config.h"
#include "math.h"
#include "status_error.hpp"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stddef.h>

using bbc = basic_block_container;

bbc::basic_block_container(){
    _begin = nullptr;
    _end = nullptr;
    _block_count = 0;
}

bbc::basic_block_container(uint8_t *frame, basic_block_config config){
    _config = config;
    uint32_t width_capacity = config.frame_width / config.block_size;
    uint32_t height_capacity = config.frame_height / config.block_size;
    _block_count = height_capacity * width_capacity;
    int bits_per_block = config.bits_per_block;
    if(_block_count % 8 != 0 || _block_count % bits_per_block != 0){
        const char *string_fmt = "The block_count (%d) must be divisible by 8 and bits_per_block(%d) without remainder!";
        char msg[256];
        sprintf(msg,string_fmt, _block_count, bits_per_block);
        throw status_error(msg, 1);
    }
    _begin = basic_block_pointer(frame, 0, &_config);
    _end = basic_block_pointer(frame, _block_count - 1, &_config);
}

bbc::iterator bbc::begin() { return _begin; }

bbc::iterator bbc::end() { return _end; }

size_t bbc::block_count() { return _block_count; }

size_t bbc::byte_count() { return  _block_count * _config.bits_per_block / 8; }

basic_block_pointer_proxy bbc::operator[](int index){ return *(_begin + index); }

basic_block_config bbc::config(){ return _config; }

size_t bbc::_get_block_count(size_t byte_size) {
    size_t count = 8 * byte_size / _config.bits_per_block;
    if(count % _config.bits_per_block > 0) count+=1;
    return count;
}

int* bbc::_convert_to_blocks(uint8_t *data, size_t block_count){
    int *blocks = new int[block_count];
    int bits_per_block = _config.bits_per_block;
    int bit0 = 0, bit1 = bits_per_block;
    for(size_t x = 0; x < block_count; x++) 
    {
        int buffer = 0;
        for(int b = bit0; b < bit1; b++) {
            int byte_index = b / 8;
            size_t bit_index = b % 8;
            uint8_t byte = data[byte_index];
            int mask = 1 << bit_index;
            int bit = ((byte & mask) >> bit_index) << b;
            buffer |= bit;
        }
        blocks[x] = buffer;
        bit0=bit1;
        bit1+=bits_per_block;
    }

    return blocks;
}

basic_block_pointer bbc::_get_end(basic_block_pointer begin, size_t byte_count){
    size_t byte_count_with_padding = get_lcd(8, _config.bits_per_block, 8 * byte_count) / 8;
    uint32_t bits_per_block = _config.bits_per_block;
    basic_block_pointer end = begin + 8 * byte_count_with_padding / _config.bits_per_block;
    if(end > _end) {
        if(end - 1 == _end) end = nullptr;
        else throw status_error("Invalid byte_count!", 1);
    }

    return end;
}

bit_area bbc::_read_block(uint8_t *bytes, bit_area area, int block) {
    //bd - bit data, bb - bit block
    size_t bit0 = area.bit0, bit1 = area.bit1;
    for(size_t bd = bit0; bd < bit1; bd++){ 
        size_t bb = bd-bit0;
        uint8_t bit = (uint8_t)(block & (1 << bb)) >> bb;

        size_t byte_index = bd / 8;
        size_t bit_index = bd % 8;
        uint8_t byte = bytes[byte_index];
        byte |= bit << bit_index;
        bytes[byte_index] = byte;
    }
    area.bit0 = bit1;
    area.bit1 += _config.bits_per_block;
    return area;
}


basic_block_pointer bbc::read(basic_block_pointer begin, uint8_t *data_out, size_t byte_count){
    basic_block_pointer end = _get_end(begin, byte_count);
    size_t blocks_to_read = _get_block_count(byte_count);
    
    int index1 = blocks_to_read - 1;
    bit_area area = { .bit0 = 0, .bit1 = _config.bits_per_block };
    for(int i = 0; i < blocks_to_read; i++) {

        int block = *(begin + i);
        if(i == blocks_to_read - 1){
            area.bit1 = 8 * byte_count;
        }
        area = _read_block(data_out, area, block);
    }
    return end;
}

basic_block_pointer bbc::write(basic_block_pointer begin, uint8_t *data_src, size_t byte_count){
    basic_block_pointer end = _get_end(begin, byte_count);
    size_t blocks_to_write = _get_block_count(byte_count);
    int *blocks = _convert_to_blocks(data_src, blocks_to_write);
    for(int i = 0; i < blocks_to_write; i++){
        *(begin + i) = blocks[i];
    }
    delete[] blocks;
    return end;
}