#include "encoder/provider/len_reader/raw_ip_length_reader.hpp"
#include "encoder/provider/basic_block/basic_block_pointer.hpp"
#include <cstdint>

raw_ip_length_reader::raw_ip_length_reader(basic_block_pointer package_begin){
    _bits_per_number = _package_begin.config().codec->bits_per_number();
    _package_begin = package_begin;
}

int raw_ip_length_reader::_get_first(int block_data, int shift){
    int minor_shift = _bits_per_number - shift;
    int mask = 0xffffffff >> (32 - minor_shift);
    return block_data & mask;
}

int raw_ip_length_reader::_get_last(int block_data, int shift){
    int block_mask = 0xffffffff >> (32 - _bits_per_number);
    int major_shift = _bits_per_number - shift;
    int mask = block_mask >> major_shift;
    return block_data & mask;
}

int raw_ip_length_reader::read(const char *frame, int frame_size) {
    uint16_t package_length = 0;
    int bit_index1 = _LENGTH_BIT_SHIFT + _LENGTH_BIT_SIZE;
    int first_shift = _LENGTH_BIT_SHIFT % _bits_per_number, last_shift = bit_index1 % _bits_per_number;
    int index0 = _LENGTH_BIT_SHIFT / _bits_per_number + (first_shift > 0 ? 1 : 0), index1 = bit_index1 / _bits_per_number + (last_shift > 0 ? 1 : 0);

    for(int i=index0; i<=index1; i++){
        int data = *(_package_begin + i);
        if(i == index0){
            data = _get_first(data, first_shift);
        }
        else if(i == index1){
            data = _get_last(data, last_shift);
            data <<= 16 - last_shift;
        }
        else{
            data <<= _bits_per_number * i;
        }
        package_length |= data;
    }
    return package_length;
}

raw_ip_length_reader::~raw_ip_length_reader(){}