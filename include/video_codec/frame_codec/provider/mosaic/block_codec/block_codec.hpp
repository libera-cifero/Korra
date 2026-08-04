#pragma once
#include "config/data/block_codec_config.hpp"

struct point {
    int x;
    int y;
};

class block_codec {
protected:
    block_codec_config $config;
    void $block_index_to_area(int block_index, point &begin, point &end);
public:
    block_codec(block_codec_config &config);
    int bits_per_number();
    int block_size();
    block_codec_config config();
    virtual void encode(char *frame, int number, int block_index) = 0;
    virtual int decode(char *frame, int block_index) = 0;

    virtual ~block_codec();
};