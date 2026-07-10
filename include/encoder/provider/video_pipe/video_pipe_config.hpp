#include <string>
#if __linux__
#include <sys/socket.h>
#elif _WIN32
#include <winsock2.h>
#endif

struct video_pipe_config {
    int frame_width;
    int frame_height;
    std::string device_name;
    sockaddr socket;
    int fps;
};