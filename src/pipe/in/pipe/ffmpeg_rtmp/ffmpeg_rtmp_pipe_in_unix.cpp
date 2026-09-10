#include "pipe/in/pipe/ffmpeg_rtmp/ffmpeg_rtmp_pipe_in_unix.hpp"
#include "lib/log.hpp"
#include "lib/args.hpp"
#include <csignal>
#include <cstddef>
#include <cstring>
#include <fcntl.h>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <sys/eventfd.h>
#include <sys/wait.h>
#include <unistd.h>

void ffmpeg_rtmp_pipe_in_unix::_launch_subprocess(){
    int args_count;
    char **args = split_args(__cmd, args_count);
    dup2(_pipe_fd[1], STDOUT_FILENO);
    close(_pipe_fd[0]);
    close(_pipe_fd[1]);
    execvp(args[0], args);
    exit(0);
}

ffmpeg_rtmp_pipe_in_unix::ffmpeg_rtmp_pipe_in_unix(ffmpeg_rtmp_config &config) : ffmpeg_rtmp_pipe_in(config){
    pipe(_pipe_fd);
    _pid = fork();
    if(_pid == 0) _launch_subprocess();
    
    _file_descriptor = _pipe_fd[0];
    int flags = fcntl(_file_descriptor, F_GETFL, 0);
    int res = fcntl(_file_descriptor, F_SETFL, flags | O_NONBLOCK);
    if(flags == -1 || res == -1) {
        throw std::runtime_error("ffmpeg_rtmp_pipe_in_unix fcntl");
    }
}


int ffmpeg_rtmp_pipe_in_unix::__read_async(){
    return ::read(_file_descriptor, __buffer + __bytes_readed, __frame_size);
}

ffmpeg_rtmp_pipe_in_unix::~ffmpeg_rtmp_pipe_in_unix(){
    auto prefix = get_method_prefix("ffmpeg_rtmp_pipe_in_unix.~ffmpeg_rtmp_pipe_in_unix");
    spdlog::debug("{} destructing...", prefix);
    spdlog::debug("{} Killing the PID {}...", prefix, _pid);

    int kill_result = kill(_pid, SIGTERM);
    spdlog::debug("{} kill returned {} (0 = success, -1 = error)", prefix, kill_result);

    usleep(100000);
    int wait_result = waitpid(_pid, NULL, WNOHANG);
    spdlog::debug("{} waitpid() returned {} (0 = still alive, > 0 = closed)", prefix, wait_result);
    close(_file_descriptor);
    spdlog::debug("{} ffmpeg_rtmp_pipe_in_unix was destructed!", prefix);
}