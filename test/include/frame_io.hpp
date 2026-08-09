#pragma once
#include "frame_meta.hpp"
#include "config/parser_factory/frame_codec_parser_static_factory.hpp"
#include "frame_expected_out_parser.hpp"
#include <functional>
#include <string>
using namespace std;
#define ITER_ACTION_ARGS const char* test_name, frame_expected_out meta, uint8_t *data, string file_name
using iter_action = function<void(ITER_ACTION_ARGS)>;


class frame_io{
private:
    frame_expected_out_parser *_parser;
public:
    frame_io();
    
    void write_frame_expected(frame_expected_out config, const string &path);
    frame_expected_out read_frame_expected(const string &path);

    void write_frame_data(uint8_t *data, int frame_width, int frame_height, const string &path);
    uint8_t *read_frame_data(const string &path, int &width, int &height);

    void iterate_frame_test_cases(const char *test_name, string subdirectory, iter_action test);

    char *convert_blocks_to_data(vector<int> &blocks, int bits_per_block);
    
    frame_codec *read_codec_from_file(string &path, video_config context);
    ~frame_io();
};

