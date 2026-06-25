#pragma once
#include <cstdint>
#include "encoder/provider/basic_block/basic_block_pointer.hpp"
#include "encoder/provider/basic_block_config.h"

class basic_block_container {
private:
    uint8_t *_frame;
    basic_block_pointer _begin;
    basic_block_pointer _end;
    basic_block_config _config;

    uint32_t _block_count;
    uint32_t _block_capacity;
public:
    using iterator = basic_block_pointer;
    basic_block_container(uint8_t *frame, basic_block_config config);
    iterator begin();
    iterator end();
    size_t count();
};