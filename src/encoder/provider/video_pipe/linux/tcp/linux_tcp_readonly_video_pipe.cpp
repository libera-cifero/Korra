#include "encoder/provider/video_pipe/linux/tcp/linux_tcp_readonly_video_pipe.hpp"
using ltrvp = linux_tcp_readonly_video_pipe;

ltrvp::linux_tcp_readonly_video_pipe(video_pipe_config config) : readonly_video_pipe(config) 
{

}
uint8_t* ltrvp::read(int size) {

}