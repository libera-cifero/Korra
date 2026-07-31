#pragma once
#include "video_encoder/frame_encoder/provider/mosaic/mosaic_settings.hpp"
#include <vector>
#include <string>

using namespace std;

struct frame_meta : mosaic_settings {
    string frame_path;
    vector<int> blocks;
};

struct frame_gen_args : mosaic_settings { 
    string expected_name;
    string frame_name;
    string color_codec_path;
};