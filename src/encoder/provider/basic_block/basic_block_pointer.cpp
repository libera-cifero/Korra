#include "encoder/provider/basic_block/basic_block_pointer_proxy.hpp"
#include "encoder/provider/basic_block/basic_block_pointer.hpp"
#include "encoder/provider/basic_block_config.h"
#include "color.h"
#include <cstddef>
#include <cstdint>

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

bbp::basic_block_pointer(uint8_t *blocks, uint32_t block_index, basic_block_config *config) {
    _blocks = blocks;
    _block_index = block_index;
    _config = config;
    _width_capacity = config->frame_width / config->block_size;
    _rect = _get_rect();
}

basic_block_pointer_proxy bbp::operator*() {
    proxy_config config = {
        .blocks = _blocks,
        .block_index=_block_index,
        .width_capacity = _width_capacity,
        .area = &_rect,
        .base = _config
    };

    return basic_block_pointer_proxy(config);
}

basic_block_pointer& bbp::operator+=(int delta_index) 
{
    _block_index += delta_index;
    return *this;;
}

basic_block_pointer& bbp::operator-=(int delta_index) 
{
    _block_index += delta_index;
    return *this;
}

basic_block_pointer operator+(const basic_block_pointer& ref, int delta_index) {
    return basic_block_pointer(ref._blocks, ref._block_index + delta_index, ref._config);
}

basic_block_pointer operator+(int delta_index, const basic_block_pointer& ref) {
    return basic_block_pointer(ref._blocks, ref._block_index + delta_index, ref._config);
}

basic_block_pointer operator-(const basic_block_pointer& ref, int delta_index) {
    return basic_block_pointer(ref._blocks, ref._block_index - delta_index, ref._config);
}

basic_block_pointer& bbp::operator++()
{
    ++_block_index;
    return *this;
}
basic_block_pointer& bbp::operator--()
{
    --_block_index;
    return *this;
}

basic_block_pointer bbp::operator++(int)
{
    basic_block_pointer tmp = *this;
    this->_block_index++;
    return tmp;
}
basic_block_pointer bbp::operator--(int)
{
    basic_block_pointer tmp = *this;
    this->_block_index--;
    return tmp;
}

bool bbp::_compare(
    const basic_block_pointer& b_ptr,
    bool (*equals)(uint32_t a, uint32_t b), 
    bool (*less)(size_t delta_base, uint32_t a, uint32_t b), 
    bool (*more)(size_t delta_base, uint32_t a, uint32_t b)
)
{
    if(_blocks == b_ptr._blocks){
        return equals(_block_index, b_ptr._block_index);
    }
    else if(_blocks < b_ptr._blocks){
        size_t block_delta_bits = 8 * (size_t)(b_ptr._blocks - _blocks);
        return less(block_delta_bits, _block_index, b_ptr._block_index);
    }
    else{
        size_t block_delta_bits = 8 * (size_t)(_blocks - b_ptr._blocks);
        return more(block_delta_bits, b_ptr._block_index, _block_index);
    }
}

bool bbp::operator==(const basic_block_pointer& ref) {
    return _compare(ref, 
        [](uint32_t a, uint32_t b){
            return a == b;
        },
        [](size_t delta_base,uint32_t a, uint32_t b) {
            return delta_base == (a - b);
        },
        [](size_t delta_base,uint32_t a, uint32_t b){
            return delta_base == (b - a);
        }
    );
}

bool bbp::operator!=(const basic_block_pointer& ref){
    return !(*this == ref);
}

bool bbp::operator<(const basic_block_pointer& ref) {
    return _compare(ref, 
        [](uint32_t a, uint32_t b){
            return a < b;
        },
        [](size_t delta_base,uint32_t a, uint32_t b) {
            return (delta_base + a) < b;
        },
        [](size_t delta_base,uint32_t a, uint32_t b){
            return (delta_base + b) < a;
        }
    );
}

bool bbp::operator>(const basic_block_pointer& ref){
    return _compare(ref, 
        [](uint32_t a, uint32_t b){
            return a > b;
        },
        [](size_t delta_base,uint32_t a, uint32_t b) {
            return (delta_base + a) > b;
        },
        [](size_t delta_base,uint32_t a, uint32_t b){
            return (delta_base + b) > a;
        }
    );
}

bool bbp::operator<=(const basic_block_pointer& ref){
    return _compare(ref, 
        [](uint32_t a, uint32_t b){
            return a <= b;
        },
        [](size_t delta_base,uint32_t a, uint32_t b) {
            return (delta_base + a) <= b;
        },
        [](size_t delta_base,uint32_t a, uint32_t b){
            return (delta_base + b) <= a;
        }
    );
}

bool bbp::operator>=(const basic_block_pointer& ref){
    return _compare(ref, 
        [](uint32_t a, uint32_t b){
            return a >= b;
        },
        [](size_t delta_base,uint32_t a, uint32_t b) {
            return (delta_base + a) >= b;
        },
        [](size_t delta_base,uint32_t a, uint32_t b){
            return (delta_base + b) >= a;
        }
    );
}