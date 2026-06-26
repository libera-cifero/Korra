#include "encoder/provider/len_reader/raw_ip_length_reader.hpp"
#include "encoder/provider/basic_block/basic_block_container.hpp"
#include <cstdint>

raw_ip_length_reader::raw_ip_length_reader(basic_block_config config){
    _config = config;
}

int raw_ip_length_reader::_get_first(int block_data, int shift){
    int minor_shift = _config.bits_per_block - shift;
    int mask = 0xffffffff >> (32 - minor_shift);
    return block_data & mask;
}

int raw_ip_length_reader::_get_last(int block_data, int shift){
    int bits_per_block = _config.bits_per_block;
    int block_mask = 0xffffffff >> (32 - bits_per_block);
    int major_shift = bits_per_block - shift;
    int mask = block_mask >> major_shift;
    return block_data & mask;
}

int raw_ip_length_reader::read(const char *frame, int frame_size) {
    uint16_t package_length = 0;
    int bits_per_block = _config.bits_per_block;
    int bit_index1 = _LENGTH_BIT_SHIFT + _LENGTH_BIT_SIZE;
    int first_shift = _LENGTH_BIT_SHIFT % bits_per_block, last_shift = bit_index1 % bits_per_block;
    int index0 = _LENGTH_BIT_SHIFT / bits_per_block + (first_shift > 0 ? 1 : 0), index1 = bit_index1 / bits_per_block + (last_shift > 0 ? 1 : 0);
    basic_block_container container((uint8_t*)frame, _config);
    for(int i=index0; i<=index1; i++){
        int data = container[i];
        if(i == index0){
            data = _get_first(data, first_shift);
        }
        else if(i == index1){
            data = _get_last(data, last_shift);
            data <<= 16 - last_shift;
        }
        else{
            data <<= bits_per_block * i;
        }
        package_length |= data;
    }
    return package_length;
}