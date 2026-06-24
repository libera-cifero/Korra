#pragma once
#include "data_length_reader.hpp"
#include "../basic_block_container.hpp"

class raw_ip_length_reader : public data_length_reader
{
private:
    const int _LENGTH_BIT_SHIFT = 16;
    const int _LENGTH_BIT_SIZE = 16;
    bbc_config _config;

    int _get_first(int block_data, int shift);
    int _get_last(int block_data, int shift);
public:
    raw_ip_length_reader(bbc_config config);
    int read(const char *first_frame, int frame_size) override;
    ~raw_ip_length_reader() override;
};