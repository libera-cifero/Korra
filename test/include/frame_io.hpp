#pragma once
#include "frame_meta.hpp"
#include <functional>
#include <string>
using namespace std;
#define ITER_ACTION_ARGS const char* test_name, frame_meta meta, uint8_t *data, string file_name
using iter_action = function<void(ITER_ACTION_ARGS)>;

void write_frame_expected(frame_meta config, const string &path);
frame_meta read_frame_expected(const string &path);

void write_frame_data(uint8_t *data, int frame_width, int frame_height, const string &path);
uint8_t *read_frame_data(const string &path, int &width, int &height);

void iterate_frame_test_cases(const char *test_name, string subdirectory, iter_action test);

char *convert_blocks_to_data(vector<int> &blocks, int bits_per_block);