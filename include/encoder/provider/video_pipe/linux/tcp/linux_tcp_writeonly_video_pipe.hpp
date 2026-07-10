#pragma once
#include "encoder/provider/video_pipe/writeonly_video_pipe.hpp"
#include <cstdint>
#include <linux/videodev2.h>
class linux_tcp_writeonly_video_pipe : public writeonly_video_pipe {
private:
    int _file_descriptor;
    v4l2_format _get_format(int width, int height);
public:
    linux_tcp_writeonly_video_pipe(video_pipe_config config);
    void write(uint8_t *data, int size) override;
    ~linux_tcp_writeonly_video_pipe() override;
};