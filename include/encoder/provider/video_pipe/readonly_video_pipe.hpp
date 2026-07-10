#include <cstdint>
class readonly_video_pipe {
public:
    uint8_t *read(int size);
};