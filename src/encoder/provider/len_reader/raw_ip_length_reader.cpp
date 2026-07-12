#include "encoder/provider/len_reader/raw_ip_length_reader.hpp"
#include "encoder/provider/basic_block/basic_block_pointer.hpp"
#include <cstdint>

raw_ip_length_reader::raw_ip_length_reader(basic_block_pointer package_begin){
    _package_begin = package_begin;
    auto config = _package_begin.config();
    _bits_per_number = _package_begin.config().codec->bits_per_number();
}

int raw_ip_length_reader::_get_first(int block_data, int shift){
    return block_data << shift;
}

int raw_ip_length_reader::_get_last(int block_data, int shift){
    int mask = (1 << shift) - 1;
    int value = block_data & mask;
    return value;
}

int raw_ip_length_reader::read(const char *frame, int frame_size) {
    uint16_t package_length = 0;
    int bit_index1 = _LENGTH_BIT_SHIFT + _LENGTH_BIT_SIZE;
    int first_shift = _LENGTH_BIT_SHIFT % _bits_per_number, last_shift = bit_index1 % _bits_per_number;
    int index0 = _LENGTH_BIT_SHIFT / _bits_per_number, index1 = bit_index1 / _bits_per_number;

    for(int i=index0; i<=index1; i++){
        int data = *(_package_begin + i);
        if(i == index0){
            data >>= first_shift;
        }
        else if(i == index1){
            data = _get_last(data, last_shift);
            data <<= 16 - last_shift;
        }
        else{
            data <<= _bits_per_number * (i - index0) - first_shift;
        }
        package_length |= data;
    }
    return package_length;
}

raw_ip_length_reader::~raw_ip_length_reader(){}