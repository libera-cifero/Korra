//input: frame_width, frame_height, bits_per_block, block_size, name of expected data file, name of frame file
//output: array of random expected blocks, path to frame in binary format
#include "color.h"
#include "encoder/provider/basic_block/rgb_index.h"
#include "math.h"
#include "encoder/provider/basic_block_config.h"
#include "encoder/provider/basic_block/rect.h"
#include "frame_io.hpp"
#include "io.hpp"
#include "json.hpp"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <stdint.h>
#include <vector>

using namespace std;
using namespace filesystem;
using json = nlohmann::json;

int block_count, width_capacity, height_capacity;

struct input : basic_block_config { char *expected_name; char *frame_name; };
struct output {
    vector<int> blocks;
    uint8_t *frame;
};

input parse_argv(int argc, char **argv) {
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
    int bits_per_block = atoi(argv[3]);
    if(bits_per_block <= 0 || bits_per_block > 24) {
        throw runtime_error("Invalid bits_per_block (third argument)!");
    }
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
    
    input res;
    res.frame_width = frame_width;
    res.frame_height = frame_height;
    res.block_size = block_size;
    res.bits_per_block = bits_per_block;
    res.expected_name = argv[5];
    res.frame_name = argv[6];
    return res;
}

output generate(input in){
    srand(time(NULL));
    vector<int> blocks(block_count);
    uint8_t *data = alloc_by_config(in);
    int max = 1 << in.bits_per_block;
    for(int i = 0; i < block_count; i++) {
        int block_data = rand() % max;
        blocks[i] = block_data;
        int color = number_to_color(block_data, in.bits_per_block);
        uint8_t r = get_r(color), g = get_g(color), b = get_b(color);
        rect area = get_rect(i, in.block_size, width_capacity);
        for(int y = area.y0; y < area.y1; y++){
            for(int x = area.x0; x < area.x1; x++) {
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
    input in;
    try{
        in = parse_argv(argc, argv);
    }
    catch (exception &e){
        cout << e.what() << endl;
        return -1;
    }

    output out = generate(in);
    string frame_expected_path = EXPECTED_PATH / in.expected_name;
    string frame_data_path = DATA_PATH / in.frame_name;
    write_frame_expected({in, in.frame_name, out.blocks}, frame_expected_path);
    write_frame_data(out.frame, in.frame_width, in.frame_height, frame_data_path);
    delete[] out.frame;
    return 0;
}