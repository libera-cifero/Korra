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

bbp::basic_block_pointer(std::nullptr_t) {
    _is_null = true;
}

bbp::basic_block_pointer():basic_block_pointer(nullptr) { }

bbp::basic_block_pointer(uint8_t *blocks, uint32_t block_index, basic_block_config *config) {
    _is_null = false;
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
    bool (*compare)(size_t a, size_t b)
)
{
    size_t a = 8 * (size_t)_blocks + _config->bits_per_block * _block_index;
    size_t b = 8 * (size_t)b_ptr._blocks + b_ptr._config->bits_per_block * b_ptr._block_index;
    return compare(a,b);
}

bool bbp::operator==(const basic_block_pointer& ref) 
{
    if(_is_null && ref._is_null) return true;
    else if(_config != ref._config) return false;
    return _compare(ref, [](size_t a, size_t b){return  a==b;});
}

bool bbp::operator!=(const basic_block_pointer& ref){
    return !(*this == ref);
}

bool bbp::operator<(const basic_block_pointer& ref) {
    if(_is_null || ref._is_null) return false;

    return _compare(ref, [](size_t a, size_t b){ return  a < b; });
}

bool bbp::operator>(const basic_block_pointer& ref) {
    if(_is_null || ref._is_null) return false;
    return _compare(ref, [](size_t a, size_t b){return  a > b;});
}

bool bbp::operator<=(const basic_block_pointer& ref) {
    if(_is_null && ref._is_null) return true;
    else if(_is_null || ref._is_null) return false;
    return _compare(ref, [](size_t a, size_t b){return  a <= b;});
}

bool bbp::operator>=(const basic_block_pointer& ref){
    if(_is_null && ref._is_null) return true;
    else if(_is_null || ref._is_null) return false;

    return _compare(ref, [](size_t a, size_t b){return  a >= b;});
}