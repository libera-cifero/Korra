#pragma once

#include "encoder/provider/basic_block_config.h"
#include "basic_block_pointer_proxy.hpp"
#include "rect.h"
#include <cstdint>

class basic_block_pointer {
private:
    uint8_t *_blocks;
    uint32_t _block_index;

    basic_block_config *_config;
    uint32_t _width_capacity;

    rect _rect;
    rect _get_rect();
public:
    basic_block_pointer(uint8_t *blocks, uint32_t block_index, basic_block_config *config);

    basic_block_pointer_proxy& operator*();

    basic_block_pointer& operator+=(int delta_index);
    basic_block_pointer& operator-=(int delta_index);

    friend basic_block_pointer operator+(basic_block_pointer ref, int delta_index);
    friend basic_block_pointer operator+(int delta_index, basic_block_pointer ref);

    friend basic_block_pointer operator-(basic_block_pointer ref, int delta_index);
    friend basic_block_pointer operator-(int delta_index, basic_block_pointer ref);

    //prefix operators
    basic_block_pointer& operator--();
    basic_block_pointer& operator++();

    //postfix operators
    basic_block_pointer operator++(int);
    basic_block_pointer operator--(int);

    bool operator==(const basic_block_pointer& ref);
    bool operator!=(const basic_block_pointer& ref);

    bool operator<(const basic_block_pointer& ref);
    bool operator>(const basic_block_pointer& ref);
    bool operator<=(const basic_block_pointer& ref);
    bool operator>=(const basic_block_pointer& ref);
};