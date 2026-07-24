#include "video/encoder/provider/mosaic_provider.hpp"
#include "frame_io.hpp"
#include "test.hpp"
#include <cstdint>

char *convert_blocks_to_data(vector<int> &blocks, int bits_per_block){
    int byte_length = blocks.size() / bits_per_block;
    char *bytes = (char*)calloc(byte_length, 1);
    for(int i = 0; i < blocks.size(); i++) {
        int block = blocks[i];
        for(int b = 0; b < bits_per_block; b++) {
            int bit_global = i * bits_per_block + b;
            int byte_index = bit_global / 8, bit_index = bit_global % 8;
            char bit = (char)(((block >> b) & 1) << bit_index);
            bytes[byte_index] |= bit;
        }
    }
    return bytes;
}

void test_to_frame() {
    const char *test_name = "mosaic_provider_test.test_to_frame";
    printInfo(test_name);
    iterate_frame_test_cases(test_name, [](const char *test_name, frame_meta expected, uint8_t *data, string file_name) 
    {
        char *payload = convert_blocks_to_data(expected.blocks, expected.codec->bits_per_number());
        mosaic_provider provider(&expected);
        uint8_t *frame = reinterpret_cast<uint8_t*>(provider.to_frame(payload));
        delete [] payload;
        for(int i = 0; i < provider.frame_size(); i++){
            uint8_t expected = data[i], fact = frame[i];
            if(expected != fact){
                delete [] data;
                delete [] frame;
                provider.~mosaic_provider();
                fail(test_name, "frame[%d] expected %d, but got %d!", 1, i, expected, fact);
            }
        }
    });

    printPass(test_name);
}

void test_to_payload(){

}

int main(){
    test_to_frame();
    test_to_payload();
    return 0;
}