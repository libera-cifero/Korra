#include "config/data/mosaic_config.hpp"
#include "config/data/palette_codec_config.hpp"
#include "video_codec/frame_codec/provider/mosaic/mosaic_settings.hpp"
#include "video_codec/frame_codec/provider/mosaic_provider.hpp"
#include "video_codec/frame_codec/provider/mosaic/block_codec/rgb_palette_codec.hpp"
#include "io.hpp"
#include "frame_io.hpp"
#include "test.hpp"
#include "time.hpp"
#include <cstdint>
#include <cstring>
#include <ctime>
#include <vector>
#include <random>

using namespace filesystem;

std::mt19937 *init_random(){
    std::random_device dev;
    std::seed_seq seq{
        dev(),   
        static_cast<uint32_t>(std::time(nullptr)),
        dev()
    };
    return new std::mt19937(seq);
}


int *get_random_rgb_palette(int bits_per_number) {
    auto random = init_random();
    int count = 1 << bits_per_number;
    int *palette = new int[count];
    for(int i = 0; i < count; i++){
        palette[i] = (*random)() % 0xFFFFFF;
    }
    delete random;
    return palette;
}

vector<mosaic_settings*>* get_rgb_palette_test_settings(int count, mosaic_config config, int bits_per_number){
    vector<mosaic_settings*> *settings_list = new vector<mosaic_settings*>;

    for(int i = 0; i < count; i++){
        mosaic_settings *settings = new mosaic_settings;
        memcpy((mosaic_config*)settings, &config, sizeof(mosaic_config));
        palette_codec_config<int> rgb_config;
        memcpy((mosaic_config*)&rgb_config, &config, sizeof(mosaic_config));
        rgb_config.bits_per_number = bits_per_number;
        rgb_config.palette = get_random_rgb_palette(bits_per_number);
        settings->codec = new rgb_palette_codec(rgb_config);
        settings_list->push_back(settings);
    }

    return settings_list;
}

char *generate_random_payload(int count) {
    auto random = init_random();
    char *data = new char[count];
    for(int i = 0; i < count; i++) data[i] = (*random)();
    delete random;
    return data;
}

void test_settings_list(const char *test_name, vector<mosaic_settings*>* settings_list){
    bool is_cheating = false, is_break = false;
    int invalid_byte_index = -1;

    for(int i = 0; i < settings_list->size() && !is_break; i++){
        mosaic_settings *settings = (*settings_list)[i];
        mosaic_provider p(settings);
        char *payload0 = generate_random_payload(p.payload_size());
        timespec t0, t1;
        clock_gettime(CLOCK_MONOTONIC, &t0);
        char *frame = p.to_frame(payload0);
        clock_gettime(CLOCK_MONOTONIC, &t1);
        double delta_ms = get_delta_millis(t0, t1);
        int payload_size = p.payload_size(), frame_size = p.frame_size(), block_size = settings->block_size;
        double speed = payload_size / delta_ms;
        printInfo("%-10d\t%-10s\t%-10.3lf\t%-10.3lf\t%-10d\t%-10d\t%-10d\t%dx%d", i, "encode", delta_ms, speed, payload_size, frame_size, block_size, settings->frame_width, settings->frame_height);
        
        clock_gettime(CLOCK_MONOTONIC, &t0);
        char *payload1 = p.to_payload(frame);
        clock_gettime(CLOCK_MONOTONIC, &t1);
        delta_ms = get_delta_millis(t0, t1);
        speed = payload_size / delta_ms;
        printInfo("%-10d\t%-10s\t%-10.3lf\t%-10.3lf\t%-10d\t%-10d\t%-10d\t%dx%d", i, "decode", delta_ms, speed, payload_size, frame_size, block_size, settings->frame_width, settings->frame_height);
        path data_path = DATA_OUT_PATH / "frame" / "test_to_frame_to_payload" / "rgb_palette_codec" / std::format("frame_{}.bmp", i);
        write_frame_data(reinterpret_cast<uint8_t*>(frame), settings->frame_width, settings->frame_height, data_path);


        if(payload0 == payload1){
            
            is_cheating = true;
            is_break = true;
        }

        for(int b = 0; b < p.payload_size(); b++){
            if(payload0[b] != payload1[b]){
                is_break = true;
                invalid_byte_index = i;
            }
        }
        delete [] frame;
        delete [] payload0;
        delete [] payload1;
    }

    delete settings_list;
    if(is_cheating){
        fail(test_name, "You are looser. You are so stupid...", 1);
    }
    if(invalid_byte_index > -1){
        fail(test_name, "Invalid byte by index %d", 2);
    }
}

void make_output_dir_if_not_exists(string block_codec_name){
    path p = DATA_OUT_PATH;
    if(!is_directory(p)) create_directory(p);
    p /= "frame";
    if(!is_directory(p)) create_directory(p);
    p /= "test_to_frame_to_payload";
    if(!is_directory(p)) create_directory(p);
    p /= block_codec_name;
    if(!is_directory(p)) create_directory(p);
}

void test_to_frame_to_payload(){
    const char *test_name = "mosaic_provider_test.test_to_frame_to_payload";
    printInfo(test_name);

    make_output_dir_if_not_exists("rgb_palette_codec");

    printInfo("%-10s\t%-10s\t%-10s\t%-10s\t%-10s\t%-10s\t%-10s\t%-10s", "FRAME №", "MODE", "TIME [ms]", "SPEED [Kib/s]", "PAYLOAD [bytes]", "FRAME [bytes]", "BLOCK_SIZE [px]", "FRAME_RESOLUTION");

    mosaic_config case_0;
    case_0.frame_width = 1280;
    case_0.frame_height = 720;
    case_0.block_size = 16;
    case_0.fps = 30;
    vector<mosaic_settings*> *settings_list = get_rgb_palette_test_settings(10, case_0, 5);
    test_settings_list(test_name, settings_list);

    mosaic_config case_1;
    case_0.frame_width = 1920;
    case_0.frame_height = 1080;
    case_0.block_size = 16;
    case_0.fps = 30;
    settings_list = get_rgb_palette_test_settings(10, case_0, 5);
    test_settings_list(test_name, settings_list);

    printPass(test_name);
}

int main(){
    test_to_frame_to_payload();
    return 0;
}