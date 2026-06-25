#pragma once
#include <cstdint>
#include <iterator>
#include <cstddef>
#include "encoder/provider/basic_block_config.h"
#include "basic_block_pointer.hpp"

class basic_block_container {
private:
    uint8_t *_frame;

    basic_block_config _config;

    uint32_t _block_count;
    uint32_t _block_capacity;
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = int;
    using difference_type = std::ptrdiff_t;
    using pointer = basic_block_pointer;
    using reference = basic_block_pointer;


    basic_block_container(uint8_t *frame, basic_block_config config);
    uint32_t count();
    uint32_t capacity();
    uint32_t bits_per_block();
    uint32_t operator[](int block_index);
    void append_block(uint32_t data);
};