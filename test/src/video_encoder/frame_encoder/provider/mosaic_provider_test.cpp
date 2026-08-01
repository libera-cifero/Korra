#include "video_encoder/frame_encoder/provider/mosaic/mosaic_settings.hpp"
#include "video_encoder/frame_encoder/provider/mosaic_provider.hpp"
#include "frame_io.hpp"
#include "test.hpp"
#include <cstdint>
#include <cstring>
#include <ctime>

double get_delta_millis(timespec t0, timespec t1){
    return ((t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec) / 1e9) * 1000.0;
}

void test_to_frame() {
    const char *test_name = "mosaic_provider_test.test_to_frame";
    printInfo(test_name);
    iterate_frame_test_cases(test_name, "random", [](const char *test_name, frame_meta expected, uint8_t *data, string file_name) 
    {
        char *payload = convert_blocks_to_data(expected.blocks, expected.codec->bits_per_number());
        mosaic_settings *settings = new mosaic_settings;
        memcpy(settings, (mosaic_settings*)&expected, sizeof(mosaic_settings));
        mosaic_provider provider(settings);

        timespec t0, t1;
        clock_gettime(CLOCK_MONOTONIC, &t0);
        uint8_t *frame = reinterpret_cast<uint8_t*>(provider.to_frame(payload));
        clock_gettime(CLOCK_MONOTONIC, &t1);
        double delta_ms = get_delta_millis(t0, t1);
        free(payload);
        for(int i = 0; i < provider.frame_size(); i++){
            uint8_t expected = data[i], fact = frame[i];
            if(expected != fact){
                delete [] data;
                delete [] frame;
                provider.~mosaic_provider();
                fail(test_name, "%s frame[%d] expected %d, but got %d!", 1, file_name.c_str(), i, expected, fact);
            }
        }
        int payload_size = provider.payload_size(), frame_size = provider.frame_size(), block_count = expected.blocks.size(), block_size = expected.block_size;
        double speed = provider.payload_size() / delta_ms;
        printInfo("%s\t%.3lfms\t%.3lfKB/s\t%d\t%d\t%d\t%d", file_name.c_str(), delta_ms, speed, payload_size, frame_size, block_count, block_size);
        delete [] frame;
    });

    printPass(test_name);
}

void test_to_payload(){
    const char *test_name = "mosaic_provider_test.test_to_payload";
    printInfo(test_name);
    iterate_frame_test_cases(test_name, "random", [](const char *test_name, frame_meta expected, uint8_t *data, string file_name) 
    {
        char *payload_expected = convert_blocks_to_data(expected.blocks, expected.codec->bits_per_number());
        mosaic_settings *settings = new mosaic_settings;
        memcpy(settings, (mosaic_settings*)&expected, sizeof(mosaic_settings));
        mosaic_provider provider(settings);
        
        timespec t0, t1;
        clock_gettime(CLOCK_MONOTONIC, &t0);
        char *payload = provider.to_payload(reinterpret_cast<char*>(data));
        clock_gettime(CLOCK_MONOTONIC, &t1);
        double delta_ms = get_delta_millis(t0, t1);

        for(int i = 0; i < provider.payload_size(); i++){
            uint8_t expected = payload_expected[i], fact = payload[i];
            if(expected != fact){
                delete [] data;
                delete [] payload_expected;
                delete [] payload;
                provider.~mosaic_provider();
                fail(test_name, "%s frame[%d] expected %d, but got %d!", 1, file_name.c_str(), i, expected, fact);
            }
        }

        int payload_size = provider.payload_size(), frame_size = provider.frame_size(), block_count = expected.blocks.size(), block_size = expected.block_size;
        double speed = provider.payload_size() / delta_ms;
        printInfo("%s\t%.3lfms\t%.3lfKB/s\t%d\t%d\t%d\t%d", file_name.c_str(), delta_ms, speed, payload_size, frame_size, block_count, block_size);

        delete[] payload_expected;
        delete [] payload;
    });

    printPass(test_name);
}

int main(){
    test_to_frame();
    test_to_payload();
    return 0;
}