#pragma once
#include <cstdint>

class video_pipe_out {
public:
    virtual void write(uint8_t *data, int size) = 0;
    virtual ~video_pipe_out() { };
};