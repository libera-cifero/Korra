#pragma once
#include "encoder/provider/basic_block_config.h"
#include "rect.h"
#include "rgb_index.h"
#include <cstdint>

struct proxy_config{
    uint8_t *blocks;
    uint32_t block_index;
    uint32_t width_capacity;
    rect *area;
    basic_block_config *base;
};

class basic_block_pointer_proxy {
private:
    proxy_config _config;
    rgb_index _get_index_by_point(int x, int y) const;
public:
    basic_block_pointer_proxy(proxy_config& config);

    //to write
    basic_block_pointer_proxy& operator=(int data);

    //to read
    operator int() const;
};