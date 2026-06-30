#include "encoder/provider/basic_block/basic_block_container.hpp"
#include "test.h"
#include <cstdint>

void write_length(int length, basic_block_container *container_ptr) {
    basic_block_container container = *container_ptr;
    auto config = container.config();
    const int length_size = 16, point0 = 16;
    int count = length_size / config.bits_per_block;
    count += length_size % config.bits_per_block == 0 ? 0 : 1;
    int mask = (1 << config.bits_per_block) - 1;

    int block0 = point0 / config.bits_per_block;
    

    for(int i = 1; i < count; i++) 
    {
        int block = length & mask;

        mask <<= config.bits_per_block;
    }
}

void test_read(){
    const char *test_name = "rilr_test.test_read";
    basic_block_config config={
        .bits_per_block = 6,
        .block_size = 16,
        .frame_width = 1920,
        .frame_height = 1080
    };
    
    uint8_t *frame = alloc_by_config(config);
    basic_block_container container(frame, config);
    
    delete [] frame;

    printPass(test_name);
}

int main(){
    test_read();
    return 0;
}