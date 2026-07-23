#include "video/encoder/provider/basic_block/basic_block_pointer.hpp"
#include "video/encoder/provider/basic_block/basic_block_math.hpp"
#include "color.hpp"
#include <cstddef>
#include <cstdint>
#include <cstring>

using bbp = basic_block_pointer;

bbp::basic_block_pointer(std::nullptr_t) {
    _context = new basic_block_pointer_context;
    _context->is_null=true;
}

bbp::basic_block_pointer():basic_block_pointer(nullptr) { }

bbp::basic_block_pointer(uint8_t *blocks, uint32_t block_index, basic_block_settings *config) {
    _context = new basic_block_pointer_context;
    _context->is_null = false;
    _context->blocks = blocks;
    _context->block_index = block_index;
    _context->config = config;
    _context->width_capacity = get_width_capacity(config->frame_width, config->block_size);
    _context->rect = get_rect(block_index, _context->config->block_size, _context->width_capacity);
}

basic_block_pointer_proxy bbp::operator*() {
    proxy_config config = {
        .blocks = _context->blocks,
        .block_index=_context->block_index,
        .width_capacity = _context->width_capacity,
        .area = &_context->rect,
        .base = _context->config
    };

    return basic_block_pointer_proxy(config);
}

basic_block_pointer& bbp::operator+=(int delta_index) 
{
    _context->block_index += delta_index;
    return *this;
}

basic_block_pointer& bbp::operator-=(int delta_index) 
{
    _context->block_index += delta_index;
    return *this;
}

basic_block_pointer operator+(const basic_block_pointer& ref, int delta_index) {
    return basic_block_pointer(ref._context->blocks, ref._context->block_index + delta_index, ref._context->config);
}

basic_block_pointer operator+(int delta_index, const basic_block_pointer& ref) {
    return basic_block_pointer(ref._context->blocks, ref._context->block_index + delta_index, ref._context->config);
}

basic_block_pointer operator-(const basic_block_pointer& ref, int delta_index) {
    return basic_block_pointer(ref._context->blocks, ref._context->block_index - delta_index, ref._context->config);
}

basic_block_pointer& bbp::operator++()
{
    ++_context->block_index;
    return *this;
}
basic_block_pointer& bbp::operator--()
{
    --_context->block_index;
    return *this;
}

basic_block_pointer bbp::operator++(int)
{
    basic_block_pointer tmp = *this;
    this->_context->block_index++;
    return tmp;
}
basic_block_pointer bbp::operator--(int)
{
    basic_block_pointer tmp = *this;
    this->_context->block_index--;
    return tmp;
}

bool bbp::_compare(
    const basic_block_pointer& b_ptr,
    bool (*compare)(size_t a, size_t b)
)
{
    size_t a = 8 * (size_t)_context->blocks + _context->config->codec->bits_per_number() * _context->block_index;
    size_t b = 8 * (size_t)b_ptr._context->blocks + b_ptr._context->config->codec->bits_per_number() * b_ptr._context->block_index;
    return compare(a,b);
}

bool bbp::operator==(const basic_block_pointer& ref) 
{
    if(_context->is_null && ref._context->is_null) return true;
    else if(_context->config != ref._context->config) return false;
    return _compare(ref, [](size_t a, size_t b){return  a==b;});
}

bool bbp::operator!=(const basic_block_pointer& ref){
    return !(*this == ref);
}

bool bbp::operator<(const basic_block_pointer& ref) {
    if(_context->is_null || ref._context->is_null) return false;

    return _compare(ref, [](size_t a, size_t b){ return  a < b; });
}

bool bbp::operator>(const basic_block_pointer& ref) {
    if(_context->is_null || ref._context->is_null) return false;
    return _compare(ref, [](size_t a, size_t b){return  a > b;});
}

bool bbp::operator<=(const basic_block_pointer& ref) {
    if(_context->is_null && ref._context->is_null) return true;
    else if(_context->is_null || ref._context->is_null) return false;
    return _compare(ref, [](size_t a, size_t b){return  a <= b;});
}

bool bbp::operator>=(const basic_block_pointer& ref){
    if(_context->is_null && ref._context->is_null) return true;
    else if(_context->is_null || ref._context->is_null) return false;

    return _compare(ref, [](size_t a, size_t b){return  a >= b;});
}

basic_block_settings* bbp::config() { return _context->config; }

bbp::~basic_block_pointer(){
    delete _context;
}