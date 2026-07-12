#include "encoder/provider/basic_block/basic_block_pointer_proxy.hpp"
#include "encoder/provider/basic_block/basic_block_pointer.hpp"
#include "encoder/provider/basic_block/rect.h"
#include "encoder/provider/basic_block_config.hpp"
#include "color.hpp"
#include <cstddef>
#include <cstdint>
#include <cstring>

using bbp = basic_block_pointer;

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
    _rect = get_rect(block_index, _config->block_size, _width_capacity);
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
    size_t a = 8 * (size_t)_blocks + _config->codec->bits_per_number() * _block_index;
    size_t b = 8 * (size_t)b_ptr._blocks + b_ptr._config->codec->bits_per_number() * b_ptr._block_index;
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

basic_block_config bbp::config() { return *_config; }