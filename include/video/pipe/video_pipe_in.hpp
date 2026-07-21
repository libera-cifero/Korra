#pragma once
#include <cstdint>

class video_pipe_in {
public:
    virtual uint8_t *read(int size) = 0;
    virtual ~video_pipe_in(){};
};