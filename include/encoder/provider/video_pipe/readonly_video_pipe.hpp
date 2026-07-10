#pragma once
#include "encoder/provider/video_pipe/video_pipe.hpp"
#include <cstdint>
class readonly_video_pipe : public video_pipe {
public:
    readonly_video_pipe(video_pipe_config config);
    virtual uint8_t *read(int size);
};