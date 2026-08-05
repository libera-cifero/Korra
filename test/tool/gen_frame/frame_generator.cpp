//input: frame_width, frame_height, block_codec_path, block_size, name of expected data file, name of frame file
//output: array of random expected blocks, path to frame in binary format
#include "frame_meta.hpp"
#include "lib/color.hpp"
#include "video_codec/frame_codec/provider/mosaic/block_codec/block_codec.hpp"
#include "block_codec_json.hpp"
#include "video_codec/frame_codec/provider/mosaic/mosaic_settings.hpp"
#include "frame_io.hpp"
#include "io.hpp"
#include "lib/json.hpp"
#include "video_codec/frame_codec/provider/mosaic_provider.hpp"
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


static block_codec *read_block_codec(string path, int frame_width, int frame_height, int block_size) {
    string text;
    fstream codec_file(path, ios_base::in);
    ostringstream reader;
    reader << codec_file.rdbuf();
    text = reader.str();
    codec_file.close();
    json j = json::parse(text);
    return parse_block_codec(j, frame_width, frame_height, block_size);
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
    int block_size = atoi(argv[4]);
    if(block_size <= 0){
        throw runtime_error("Invalid block_size (fourth argument)!");
    }

    block_codec *codec = read_block_codec(codec_path, frame_width, frame_height, block_size);
    int bits_per_block = codec->bits_per_number();

    width_capacity = frame_width / block_size;
    height_capacity = frame_height / block_size;
    block_count = height_capacity * width_capacity;

    if(block_count == 0 || block_count % 8 != 0 || block_count % bits_per_block != 0){
        delete [] codec;
        const char *string_fmt = "The block_count (%d) must be divisible by 8 and bits_per_block(%d) without remainder and greater than 0!";
        char msg[256];
        sprintf(msg,string_fmt, block_count, bits_per_block);
        throw runtime_error(msg);
    }
    
    frame_gen_args res;
    res.frame_width = frame_width;
    res.frame_height = frame_height;
    res.block_size = block_size;
    res.codec = codec;
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
    block_codec *codec = in.codec;
    int count = codec -> numbers_count();
    for(int i = 0; i < block_count; i++) {
        int block_data = rand_int() % count;
        blocks[i] = block_data;
    }
    char *payload = convert_blocks_to_data(blocks, codec -> bits_per_number());
    auto m = new mosaic_settings;
    memcpy(m, (mosaic_settings*)&in, sizeof(mosaic_settings)); //it will be erased after leaving the method
    mosaic_provider p(m);
    char *frame = p.to_frame(payload);
    delete [] payload;
    return { .blocks = blocks, .frame = reinterpret_cast<uint8_t*>(frame) };
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
    frame_meta meta;
    meta.blocks = out.blocks;
    meta.frame_path = in.frame_name;
    memcpy((mosaic_settings*)&meta, (mosaic_settings*)&in, sizeof(mosaic_settings));
    write_frame_expected(meta, frame_expected_path);
    write_frame_data(out.frame, in.frame_width, in.frame_height, frame_data_path);
    delete in.codec;
    delete[] out.frame;
    return 0;
}