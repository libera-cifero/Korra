//input: count, palette_path
//output: lines of [width height bits_per_block block_size frame_N.json frame_N.bmp]
#include "io.hpp"
#include "frame_meta.hpp"
#include "color_codec/codec_json.hpp"
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <regex>
#include <sstream>
#include <string>
using namespace std;
using namespace filesystem;

void print_args(frame_gen_args args){
    cout 
        << args.frame_width << " " 
        << args.frame_height << " " 
        << args.color_codec_path << " "
        << args.block_size << " "
        << args.expected_name << " "
        << args.frame_name;
}

int get_max_file_index(path dir_path, regex pattern) {
    vector<int> file_indices;
    directory_iterator iter(dir_path);
    for(directory_entry entry : iter){
        if(entry.is_regular_file()){
            smatch match;
            string file_name = entry.path().filename();
            if(regex_match(file_name, match, pattern)){
                string index_str = match[1].str();
                int index = atoi(index_str.c_str());
                file_indices.push_back(index);
            }
        }
    }
    if(file_indices.empty()){
        return -1;
    }

    return *max_element(file_indices.begin(), file_indices.end());
}

int get_bits_per_number(string codec_path) {
    fstream file(DATA_COLOR_CODEC_PATH / codec_path);
    ostringstream buf;
    buf << file.rdbuf();
    string data = buf.str();
    file.close();
    json j = json::parse(data);
    auto codec = parse_color_codec(j);
    int bits_per_number = codec->bits_per_number();
    delete codec;
    return bits_per_number;
}

frame_gen_args gen_random_args(int expected_index, int data_index, int bits_per_number, string codec_path) {
    int frame_width = (rand() % 1913) + 8;
    int frame_height = (rand() % 1073) + 8;
    int block_size, block_count = 0, min_size = frame_width < frame_height ? frame_width : frame_height;
    bool bits_per_block_compatible = false;
    while(block_count == 0 || block_count % 8 != 0 || block_count % bits_per_number != 0) {
        block_size = (rand() % min_size) + 1;
        int width_capacity = frame_width / block_size;
        int height_capacity = frame_height / block_size;
        block_count = height_capacity * width_capacity;
    }

    string expected_file = "frame_" + to_string(expected_index) + ".json";
    string data_file = "frame_" + to_string(data_index) + ".bmp";

    frame_gen_args args;
    args.frame_name = data_file;
    args.expected_name = expected_file;
    args.block_size = block_size;
    args.frame_width = frame_width;
    args.frame_height = frame_height;
    args.color_codec_path = codec_path;

    return args;
}

int main(int argc, char **argv) {
    if(argc < 3) {
        cerr << "Invalid arguments count!" << endl;
        return -1;
    }

    int count = atoi(argv[1]);
    if(count <= 0){
        cerr << "Invalid count " << count << "!" << endl;
        return -2;
    }

    int expected_index = get_max_file_index(EXPECTED_FRAME_PATH, regex("frame_([0-9]+)\\.json"));
    int data_index = get_max_file_index(DATA_FRAME_PATH, regex("frame_([0-9]+)\\.bmp"));

    string codec_path = argv[2];
    int bits_per_number = get_bits_per_number(codec_path);

    srand(time(NULL));
    for(int i = 0; i < count; i++){
        frame_gen_args args = gen_random_args(++expected_index, ++data_index, bits_per_number, codec_path);
        if(i > 0) cout << endl;
        print_args(args);
    }

    return 0;
}