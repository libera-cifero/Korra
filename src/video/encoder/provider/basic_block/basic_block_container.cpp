#include "video/encoder/provider/basic_block/basic_block_container.hpp"
#include "video/encoder/provider/basic_block/basic_block_pointer.hpp"
#include "video/encoder/provider/basic_block/basic_block_pointer_proxy.hpp"
#include "video/encoder/provider/basic_block/basic_block_settings.hpp"
#include "video/encoder/provider/basic_block/basic_block_math.hpp"
#include "math.hpp"
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

bbc::basic_block_container(uint8_t *frame, basic_block_settings *config){
    _config = config;
    _block_count = get_block_count(config->frame_width, config->frame_height, config->block_size);
    _bits_per_block = config->codec->bits_per_number();
    if(_block_count == 0 ||_block_count % 8 != 0 || _block_count % _bits_per_block != 0){
        const char *string_fmt = "The block_count (%d) must be divisible by 8 and bits_per_block(%d) without remainder and greater than 0!";
        char msg[256];
        sprintf(msg,string_fmt, _block_count, _bits_per_block);
        throw status_error(msg, 1);
    }
    _frame = frame;
    _begin = basic_block_pointer(_frame, 0, _config);
    _end = basic_block_pointer(_frame, _block_count - 1, _config);
}

bbc::iterator bbc::begin() { return _begin; }

bbc::iterator bbc::end() { return _end; }

size_t bbc::block_count() { return _block_count; }

size_t bbc::byte_count() { return  get_basic_block_frame_payload_size(_bits_per_block, _block_count); }

basic_block_pointer_proxy bbc::operator[](int index){ 
    basic_block_pointer new_ptr = _begin + index;
    basic_block_pointer_proxy proxy = *new_ptr;
    return proxy; 
}

basic_block_settings* bbc::config(){ return _config; }

int* bbc::_convert_to_blocks(uint8_t *data, size_t block_count){
    int *blocks = new int[block_count];
    int bits_per_block = _bits_per_block;
    int bit0 = 0, bit1 = bits_per_block;
    for(size_t x = 0; x < block_count; x++) 
    {
        int buffer = 0;
        for(int b = bit0; b < bit1; b++) {
            int byte_index = b / 8;
            size_t bit_index = b % 8;
            uint8_t byte = data[byte_index];
            int mask = 1 << bit_index;
            int bit = ((byte & mask) >> bit_index) << (b - bit0);
            buffer |= bit;
        }
        blocks[x] = buffer;
        bit0=bit1;
        bit1+=bits_per_block;
    }

    return blocks;
}

basic_block_pointer bbc::_get_end(basic_block_pointer begin, size_t byte_count){
    size_t byte_count_with_padding = get_lcm(8, _bits_per_block, 8 * byte_count) / 8;
    uint32_t bits_per_block = _bits_per_block;
    basic_block_pointer end = begin + 8 * byte_count_with_padding / _bits_per_block;
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
        uint8_t bit = (uint8_t)((block >> bb) & 1);

        size_t byte_index = bd / 8;
        size_t bit_index = bd % 8;
        uint8_t byte = bytes[byte_index];
        byte |= bit << bit_index;
        bytes[byte_index] = byte;
    }
    area.bit0 = bit1;
    area.bit1 += _bits_per_block;
    return area;
}


basic_block_pointer bbc::read(basic_block_pointer begin, uint8_t *data_out, size_t byte_count){
    memset(data_out, 0, byte_count);
    basic_block_pointer end = _get_end(begin, byte_count);
    size_t blocks_to_read = get_block_count(byte_count, _bits_per_block);

    int index1 = blocks_to_read - 1;
    bit_area area = { .bit0 = 0, .bit1 = (unsigned int)_bits_per_block };
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
    size_t blocks_to_write = get_block_count(byte_count, _bits_per_block);
    int *blocks = _convert_to_blocks(data_src, blocks_to_write);
    for(int i = 0; i < blocks_to_write; i++){
        *(begin + i) = blocks[i];
    }
    delete[] blocks;
    return end;
}

uint8_t* bbc::frame_bitmap(){ return _frame; }
void bbc::frame_bitmap(uint8_t *frame){
    delete _frame;
    _frame = frame;
}