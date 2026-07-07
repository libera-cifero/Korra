#pragma once
#include "encoder/provider/basic_block_config.hpp"
#include <vector>
#include <string>

using namespace std;

struct frame_meta : basic_block_config {
    string frame_path;
    vector<int> blocks;
};

struct frame_gen_args : basic_block_config { 
    string expected_name;
    string frame_name;
    string color_codec_path;
};