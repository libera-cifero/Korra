#pragma once
#include "config/data/video_config.hpp"
#include "video_codec/frame_codec/frame_codec.hpp"
#include <vector>
#include <string>

using namespace std;

struct frame_data {
    string path;
    vector<char> payload;
};

struct frame_expected_out : video_config {
    frame_codec *codec;
    frame_data data;
};

struct frame_expected_in : video_config {
    string codec_path;
    string data_path;
    string expected_path;
};