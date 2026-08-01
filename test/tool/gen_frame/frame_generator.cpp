//input: frame_width, frame_height, color_codec_path, block_size, name of expected data file, name of frame file
//output: array of random expected blocks, path to frame in binary format
#include "lib/color.hpp"
#include "video_encoder/frame_encoder/provider/mosaic/color_codec/color_codec.hpp"
#include "video_encoder/frame_encoder/provider/mosaic/color_codec/codec_json.hpp"
#include "video_encoder/frame_encoder/provider/mosaic/color_codec/palette_codec.hpp"
#include "video_encoder/frame_encoder/provider/mosaic/point.hpp"
#include "video_encoder/frame_encoder/provider/mosaic/mosaic_settings.hpp"
#include "frame_io.hpp"
#include "io.hpp"
#include "lib/json.hpp"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <exception>
#include <fstream>
#include <ios>
#include <iostream>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <stdint.h>
#include <vector>
#include <random>

using namespace std;
using namespace filesystem;
using json = nlohmann::json;

int block_count, width_capacity, height_capacity;

struct output {
    vector<int> blocks;
    uint8_t *frame;
};

struct rgb_index{
    int r_index;
    int g_index;
    int b_index;
};

uint8_t *alloc_by_config(mosaic_settings config){
    return (uint8_t*)malloc(3 * config.frame_width * config.frame_height);
}

struct rgb_index get_index_by_point(int frame_width, int x, int y){
    int b_pos = 3 * (y * frame_width + x);
    int g_pos = b_pos + 1, r_pos = b_pos + 2;

    struct rgb_index index = { r_pos, g_pos, b_pos };
    return index;
}

void get_index_by_point(int frame_width, point p, rgb_index &index){
    int b_pos = 3 * (p.y * frame_width + p.x);
    int g_pos = b_pos + 1, r_pos = b_pos + 2;

    index.r_index = r_pos;
    index.g_index = g_pos;
    index.b_index = b_pos;
}


static color_codec *read_color_codec(string path) {
    string text;
    fstream codec_file(path, ios_base::in);
    ostringstream reader;
    reader << codec_file.rdbuf();
    text = reader.str();
    codec_file.close();
    json j = json::parse(text);
    return parse_color_codec(j);
}

frame_gen_args parse_argv(int argc, char **argv) {
    if(argc < 7){
        throw runtime_error("Too few arguments!");
    }
    int frame_width = atoi(argv[1]);
    if(frame_width <= 0){
        throw runtime_error("Invalid frame_width (first argument)!");
    }
    int frame_height = atoi(argv[2]);
    if(frame_height <= 0){
        throw runtime_error("Invalid frame_height (second argument)!");
    }
    string codec_path = DATA_COLOR_CODEC_PATH / argv[3];
    color_codec *codec = read_color_codec(codec_path);
    int bits_per_block = codec->bits_per_number();
    int block_size = atoi(argv[4]);
    if(block_size <= 0){
        throw runtime_error("Invalid block_size (fourth argument)!");
    }

    width_capacity = frame_width / block_size;
    height_capacity = frame_height / block_size;
    block_count = height_capacity * width_capacity;

    if(block_count == 0 || block_count % 8 != 0 || block_count % bits_per_block != 0){
        const char *string_fmt = "The block_count (%d) must be divisible by 8 and bits_per_block(%d) without remainder and greater than 0!";
        char msg[256];
        sprintf(msg,string_fmt, block_count, bits_per_block);
        throw runtime_error(msg);
    }
    
    frame_gen_args res;
    res.frame_width = frame_width;
    res.frame_height = frame_height;
    res.block_size = block_size;
    res.codec = (palette_codec*)codec;
    res.expected_name = argv[5];
    res.frame_name = argv[6];
    return res;
}

uint32_t rand_int(){
    std::random_device dev;
    std::seed_seq seq{
        dev(),   
        static_cast<uint32_t>(std::time(nullptr)),
        dev()
    };
    std::mt19937 random(seq);
    return random();
}

output generate(frame_gen_args in){
    srand(time(NULL));
    vector<int> blocks(block_count);
    color_codec *codec = in.codec;
    uint8_t *data = alloc_by_config(in);
    int count = codec->color_count();
    for(int i = 0; i < block_count; i++) {
        int block_data = rand_int() % count;
        blocks[i] = block_data;
        int color = codec->number_to_color(block_data);
        uint8_t r = get_r(color), g = get_g(color), b = get_b(color);
        int index_y = i / width_capacity, index_x = i % width_capacity;
        uint32_t y0 = index_y * in.block_size, x0 = index_x * in.block_size;
        uint32_t y1 = y0 + in.block_size, x1 = x0 + in.block_size;
        for(int y = y0; y < y1; y++){
            for(int x = x0; x < x1; x++) {
                rgb_index rgb = get_index_by_point(in.frame_width, x, y);
                data[rgb.r_index] = r;
                data[rgb.g_index] = g;
                data[rgb.b_index] = b;
            }
        }
    }
    return { .blocks = blocks, .frame = data };
}

int main(int argc, char **argv) {
    frame_gen_args in;
    try{
        in = parse_argv(argc, argv);
    }
    catch (exception &e){
        cout << e.what() << endl;
        return -1;
    }

    output out = generate(in);
    string frame_expected_path = EXPECTED_FRAME_PATH / in.expected_name;
    string frame_data_path = DATA_FRAME_PATH / in.frame_name;
    write_frame_expected({in, in.frame_name, out.blocks}, frame_expected_path);
    write_frame_data(out.frame, in.frame_width, in.frame_height, frame_data_path);
    delete in.codec;
    delete[] out.frame;
    return 0;
}