#include <cstdint>
class writeonly_video_pipe {
public:
    void write(uint8_t *data, int size);
};