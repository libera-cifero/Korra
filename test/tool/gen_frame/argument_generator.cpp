//input: count, palette_path, width, height, block_size, folder. 
//Width, height, block_size and folder aren't required here. If they aren't defined, then they will be generated 

//output: lines of [width height bits_per_block block_size frame_N.json frame_N.bmp]
#include "frame_io.hpp"
#include "io.hpp"
#include "frame_meta.hpp"
#include "lib/CLI11.hpp"
#include "size_generator/size_generator.hpp"
#include "size_generator/mosaic_generator.hpp"
#include "video_codec/frame_codec/frame_codec.hpp"
#include "video_codec/frame_codec/provider/provider.hpp"
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <format>
#include <regex>
#include <stdexcept>
#include <string>
#include <regex>
#include <vector>

using namespace std;
using namespace filesystem;

void print_args(frame_expected_in args){
    printf(
        "--width %d --height %d --fps %d --frame_codec_path \"%s\" --expected_path \"%s\" --data_path \"%s\"\n", 
        args.frame_width, args.frame_height, args.fps, args.codec_path.c_str(), args.expected_path.c_str(), args.data_path.c_str()
    );
    fflush(stdout);
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

int set_expected_file_path(frame_expected_in &in, int prev_index){
    int index;
    path root = EXPECTED_FRAME_PATH / to_native_path(in.expected_path);
    if(prev_index < 0){
        regex pattern("frame_(\\d+)\\.json");
        index = get_max_file_index(root, pattern) + 1;
    }
    else index = prev_index + 1;
    
    in.expected_path = path(in.expected_path) / std::format("frame_{}.json", index);
    return index;
}

int set_data_file_path(frame_expected_in &in, int prev_index)
{
    int index;
    path root = DATA_FRAME_PATH / to_native_path(in.data_path);
    if(prev_index < 0){
        regex pattern("frame_(\\d+)\\.bmp");
        index = get_max_file_index(root, pattern) + 1;
    }
    else index = prev_index + 1;
    in.data_path = path(in.data_path) / std::format("frame_{}.bmp", index);
    return index;
}

size_generator *select_generator(frame_codec *codec, vector<size_generator*> &generators){
    provider *p = codec->get_provider();
    size_generator *generator = nullptr;
    for(auto gen : generators){
        if(gen->can_generate(p)){
            generator = gen;
            break;
        }
    }

    if(generator == nullptr) throw runtime_error("size generator is undefined!");
    return generator;
}

void set_size(size_generator *generator, frame_codec *codec, frame_expected_in &in){
    bool is_width_random = in.frame_width <= 0, is_height_random = in.frame_height <= 0;
    if(is_width_random || is_height_random){
        area_size size = {(int)in.frame_width, (int)in.frame_height};
        size = generator->random_size(codec->get_provider(), size);
        if(is_width_random) in.frame_width = size.width;
        if(is_height_random) in.frame_height = size.height;
    }
}

int main(int argc, char **argv){
    vector<size_generator*> generators = {
        new mosaic_generator
    };

    CLI::App app{"args_gen_tool"};
    argv = app.ensure_utf8(argv);
    frame_expected_in in;
    int arg_count;
    in.frame_width = 0;
    in.frame_height = 0;
    in.fps = 0;
    in.expected_path = in.data_path = "";
    app.add_option("-C,--count", arg_count)->required();
    app.add_option("-W,--width", in.frame_width);
    app.add_option("-H,--height", in.frame_height);
    app.add_option("-f,--fps", in.fps);
    app.add_option("-c,--frame_codec_path", in.codec_path)->required();
    app.add_option("-e,--expected_path", in.expected_path, "path to directory stores expected files");
    app.add_option("-d,--data_path", in.data_path, "path to directory stores data files");
    CLI11_PARSE(app, argc, argv);

    frame_io io_context;
    string codec_path = DATA_FRAME_CODEC_PATH / to_native_path(in.codec_path);
    frame_codec *codec = io_context.read_codec_from_file(codec_path, in);
    size_generator *generator = select_generator(codec, generators);
    int prev_expected_index = -1,  prev_data_index = -1;
    for(int i = 0; i < arg_count; i++)
    {
        frame_expected_in args = in;
        set_size(generator, codec, args);
        prev_expected_index = set_expected_file_path(args, prev_expected_index);
        prev_data_index = set_data_file_path(args, prev_data_index);
        print_args(args);
    }

    for(int i = 0; i < generators.size(); i++){
        delete generators[i];
    }

    delete codec;
    return 0;
}