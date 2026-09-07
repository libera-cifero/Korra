#pragma once
#include <cstdint>

class video_pipe_out {
public:
    virtual void write(char *data, int size) = 0;
    virtual ~video_pipe_out() { };
};