#include "encoder/provider/video_pipe/linux/tcp/linux_tcp_writeonly_video_pipe.hpp"
#include <fcntl.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
using ltwvp = linux_tcp_writeonly_video_pipe;

v4l2_format ltwvp::_get_format(){
    v4l2_format fmt = {0};
    int width = $config.frame_width, height = $config.frame_height;

    fmt.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;

    fmt.fmt.pix.width = width;
    fmt.fmt.pix.height = height;

    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB24;
    fmt.fmt.pix.field = V4L2_FIELD_NONE;

    fmt.fmt.pix.bytesperline = width * 3;
    fmt.fmt.pix.sizeimage = width * height * 3;
    fmt.fmt.pix.colorspace = V4L2_COLORSPACE_SRGB;
    return fmt;
}

ltwvp::linux_tcp_writeonly_video_pipe(video_pipe_config config) : writeonly_video_pipe(config) {
    _file_descriptor = open($config.device_name.c_str(), O_WRONLY);
    v4l2_format fmt = _get_format();
    ioctl(_file_descriptor, VIDIOC_S_FMT, &fmt);
}

void write(uint8_t *data, int size){
    
}

ltwvp::~linux_tcp_writeonly_video_pipe() {
    close(_file_descriptor);
}