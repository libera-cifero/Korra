#include "encoder/provider/basic_block_container.hpp"
#include "encoder/provider/len_reader/raw_ip_length_reader.hpp"
#include "test.h"
#include <cstdint>

void test_read(){
    const char *test_name = "rilr_test.test_read";
    bbc_config config={
        .bits_per_block = 6,
        .block_size = 16,
        .frame_width = 1920,
        .frame_height = 1080
    };
    
    uint8_t *frame = new uint8_t[3*config.frame_height*config.frame_width];
    
    basic_block_container container(frame, config);
    
    
    delete [] frame;

    printPass(test_name);
}

int main(){
    test_read();
    return 0;
}