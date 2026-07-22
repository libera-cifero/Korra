#pragma once
#include "len_reader/data_length_reader.hpp"
#include "video/encoder/video_frame.hpp"
#include <vector>
using namespace std;
class provider {
private:
    data_length_reader *_length_reader;
public:
    provider(data_length_reader*);
    data_length_reader *length_reader();
    //First 4 bytes of output is total size of data
    virtual char* to_data(vector<video_frame>*) = 0;
    //First 4 bytes of input is total size of data
    virtual vector<video_frame>* to_frames(char*) = 0;
    virtual ~provider();
};