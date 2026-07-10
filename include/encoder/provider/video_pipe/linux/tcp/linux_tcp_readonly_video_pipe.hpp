#pragma once
#include "encoder/provider/video_pipe/readonly_video_pipe.hpp"
#include <sys/socket.h>

class linux_tcp_readonly_video_pipe : public readonly_video_pipe {
public:
    linux_tcp_readonly_video_pipe(video_pipe_config config);
    uint8_t *read(int size) override;
};