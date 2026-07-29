#pragma once
#include "data_boxer/data/korra_data.hpp"

class piceable_data : public korra_data {
public:
    virtual void read_piece(char* piece_buffer, int piece_size, int &byte_index) = 0;
    virtual void append_piece(char *piece, int piece_size) = 0;
};