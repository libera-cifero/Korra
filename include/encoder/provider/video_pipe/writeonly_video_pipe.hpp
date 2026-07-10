#pragma once
#include "video_pipe.hpp"
#include <cstdint>
class writeonly_video_pipe : public video_pipe {
public:
    writeonly_video_pipe(video_pipe_config config);
    virtual void write(uint8_t *data, int size);
};