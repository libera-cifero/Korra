#pragma once
#include "data_length_reader.hpp"
#include "encoder/provider/basic_block/basic_block_pointer.hpp"

class raw_ip_length_reader : public data_length_reader
{
private:
    static const int _LENGTH_BIT_SHIFT = 16;
    static const int _LENGTH_BIT_SIZE = 16;
    basic_block_pointer _package_begin;
    int _bits_per_number;

    int _get_first(int block_data, int shift);
    int _get_last(int block_data, int shift);
public:
    raw_ip_length_reader(basic_block_pointer package_begin);
    int read(const char *first_frame, int frame_size) override;
    ~raw_ip_length_reader() override;
};