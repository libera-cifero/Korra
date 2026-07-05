#pragma once
#include "data_length_reader.hpp"
#include "encoder/provider/basic_block/basic_block_container.hpp"
#include "encoder/provider/basic_block_config.hpp"

class raw_ip_length_reader : public data_length_reader
{
private:
    const int _LENGTH_BIT_SHIFT = 16;
    const int _LENGTH_BIT_SIZE = 16;
    basic_block_container _container;
    basic_block_config _config;

    int _get_first(int block_data, int shift);
    int _get_last(int block_data, int shift);
public:
    raw_ip_length_reader(basic_block_container* c);
    int read(const char *first_frame, int frame_size) override;
    ~raw_ip_length_reader() override;
};