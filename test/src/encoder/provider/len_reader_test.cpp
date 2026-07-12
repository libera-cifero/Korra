#include "encoder/provider/basic_block/basic_block_pointer.hpp"
#include "encoder/provider/basic_block/basic_block_container.hpp"
#include "encoder/provider/len_reader/raw_ip_length_reader.hpp"
#include "frame_io.hpp"
#include "test.h"
#include "frame_io.hpp"
#include <cstdint>
#include <cstring>
#include <bitset>

using namespace std;
void test_raw_ip_length_reader_read() {
    const char *test_name = "len_reader_test.test_raw_ip_length_reader_read";

    iterate_frame_test_cases(test_name, [](const char *test_name, frame_meta expected, uint8_t *data, string file_name)
    {
        basic_block_container container(data, expected);
        uint16_t expected_length = 0;
        int byte_count = container.byte_count();
        uint8_t *frame_payload = new uint8_t[byte_count];
        container.read(container.begin(), frame_payload, byte_count);

        memcpy(&expected_length, frame_payload + 2, 2);

        raw_ip_length_reader reader(container.begin());
        uint16_t length = (uint16_t)reader.read(reinterpret_cast<char*>(data), 0);
        if(length != expected_length){
            bitset<16> expected_bits(expected_length);
            bitset<16> fact_bits(length);
            string expected_str = expected_bits.to_string(), fact_str = fact_bits.to_string();
            char error_text[256];
            fail(test_name, "Expected %s, but got %s!", 1, expected_str.c_str(), fact_str.c_str());
        }
    });

    printPass(test_name);
}

int main(){
    test_raw_ip_length_reader_read();
    return 0;
}