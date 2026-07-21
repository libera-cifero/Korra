#include <stdlib.h>
#include <linux/videodev2.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <sys/ioctl.h>

typedef struct v4l2_format v4l2_format;

v4l2_format get_format(int width, int height){
    v4l2_format fmt = {0};

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

char rand_byte(){
    return (rand() % 255) - 127;
}

void fill_camera_data(int fd, int width, int heigth) {
    int frame_size = width*heigth*3;
    char *frame = (char*)malloc(frame_size);
    for(int y = 0; y < heigth; y++){
        for(int x = 0; x < width; x++){
            int r = (y * width + x) * 3;
            int g = r + 1;
            int b = r + 2;

            frame[r] = rand_byte();
            frame[g] = rand_byte();
            frame[b] = rand_byte();
        }
    }

    write(fd, frame, frame_size);
    free(frame);
}

int main(){
    int width = 640, height = 480;

    v4l2_format fmt = get_format(640, 480);

    int fd = open("/dev/video10", O_WRONLY);
    ioctl(fd, VIDIOC_S_FMT, &fmt);

    srand(time(NULL));
    while(1){
        fill_camera_data(fd, width, height);
        usleep(16667);
    }
    close(fd);
    return 0;
}