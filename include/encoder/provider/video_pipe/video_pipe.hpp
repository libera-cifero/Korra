#pragma once
#if __linux__
#include <sys/socket.h>
#elif _WIN32
#include <winsock2.h>
#endif
#include "video_pipe_config.hpp"

class video_pipe {
protected:
    video_pipe_config $config;
public:
    video_pipe(video_pipe_config config);
    video_pipe_config config();
    virtual ~video_pipe() { };
};