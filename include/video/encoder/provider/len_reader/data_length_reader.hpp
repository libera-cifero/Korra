#pragma once
class data_length_reader 
{
public:
    virtual int read(const char *first_frame, int frame_size) = 0;
    virtual ~data_length_reader(){};
};