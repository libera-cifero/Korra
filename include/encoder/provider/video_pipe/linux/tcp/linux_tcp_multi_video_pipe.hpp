#pragma once
#include "linux_tcp_readonly_video_pipe.hpp"
#include "linux_tcp_writeonly_video_pipe.hpp"

class linux_tcp_multi_video_pipe : public linux_tcp_readonly_video_pipe, public linux_tcp_writeonly_video_pipe {

};