#pragma once
#include "frame_meta.hpp"
#include <string>
using namespace std;

void write_frame_expected(frame_meta config, const string &path);
frame_meta read_frame_expected(const string &path);

void write_frame_data(uint8_t *data, int frame_width, int frame_height, const string &path);
uint8_t *read_frame_data(const string &path, int &width, int &height);