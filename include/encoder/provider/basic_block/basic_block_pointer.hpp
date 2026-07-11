#pragma once

#include "encoder/provider/basic_block_config.hpp"
#include "basic_block_pointer_proxy.hpp"
#include "rect.h"
#include <iterator>
#include <cstddef>
#include <cstdint>

class basic_block_pointer {
private:
    bool _is_null;
    uint8_t *_blocks;
    uint32_t _block_index;

    basic_block_config *_config;
    uint32_t _width_capacity;

    rect _rect;
    bool _compare(
        const basic_block_pointer& b_ptr,
        bool (*compare)(size_t a, size_t b)
    );
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = int;
    using difference_type = std::ptrdiff_t;
    using pointer = basic_block_pointer;
    using reference = basic_block_pointer_proxy;

    basic_block_pointer(uint8_t *blocks, uint32_t block_index, basic_block_config *config);
    basic_block_pointer(std::nullptr_t);
    basic_block_pointer();
    basic_block_pointer_proxy operator*();

    basic_block_pointer& operator+=(int delta_index);
    basic_block_pointer& operator-=(int delta_index);

    friend basic_block_pointer operator+(const basic_block_pointer& ref, int delta_index);
    friend basic_block_pointer operator+(int delta_index, const basic_block_pointer& ref);
    friend basic_block_pointer operator-(const basic_block_pointer& ref, int delta_index);

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

    basic_block_config config();
};