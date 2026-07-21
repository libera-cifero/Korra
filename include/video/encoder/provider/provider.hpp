#pragma once
#include "len_reader/data_length_reader.hpp"
class provider {
public:
    data_length_reader *length_reader();    
    virtual char *pull_data(char **frames, int *data_count) = 0;
    virtual char **push_frames(char *data, int *frame_count) = 0;
    virtual ~provider();
};