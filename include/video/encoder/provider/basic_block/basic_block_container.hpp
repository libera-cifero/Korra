#pragma once
#include <cstddef>
#include <cstdint>
#include "video/encoder/provider/basic_block/basic_block_pointer.hpp"
#include "video/encoder/provider/basic_block/basic_block_pointer_proxy.hpp"
#include "basic_block_settings.hpp"
#include "bit_area.hpp"

class basic_block_container {
private:
    uint8_t *_frame;
    basic_block_pointer _begin;
    basic_block_pointer _end;
    basic_block_settings _config;

    uint32_t _block_count;
    int _bits_per_block;

    size_t _get_block_count(size_t byte_size);
    basic_block_pointer _get_end(basic_block_pointer begin, size_t byte_count);

    bit_area _read_block(uint8_t *bytes, bit_area area, int block);

    int* _convert_to_blocks(uint8_t *data, size_t byte_size);
public:
    using iterator = basic_block_pointer;
    basic_block_container();
    basic_block_container(uint8_t *frame, basic_block_settings config);
    iterator begin();
    iterator end();
    size_t block_count();
    size_t byte_count();
    basic_block_settings config();
    basic_block_pointer_proxy operator[](int index);

    basic_block_pointer read(basic_block_pointer begin, uint8_t *data_out, size_t byte_count);
    
    basic_block_pointer write(basic_block_pointer begin, uint8_t *data_src, size_t byte_count);
};