#include <sys/socket.h>

class video_pipe {
public:
    sockaddr *get_socket();
};