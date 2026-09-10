#include "pipe/out/pipe/ffmpeg_rtmp/ffmpeg_rtmp_pipe_out_unix.hpp"
#include "config/data/ffmpeg_rtmp_config.hpp"
#include "lib/args.hpp"
#include "lib/log.hpp"
#include <csignal>
#include <spdlog/spdlog.h>
#include <cstdio>
#include <sys/wait.h>
#include <unistd.h>

void ffmpeg_rtmp_pipe_out_unix::_launch_subprocess(){
    int args_count;
    char **args = split_args(__cmd, args_count);
    dup2(_pipe_fd[0], STDIN_FILENO);
    close(_pipe_fd[0]);
    close(_pipe_fd[1]);
    execvp(args[0], args);
    perror("execvp failed");
    exit(1);
}

ffmpeg_rtmp_pipe_out_unix::ffmpeg_rtmp_pipe_out_unix(ffmpeg_rtmp_config &config) : ffmpeg_rtmp_pipe_out(config){
    pipe(_pipe_fd);
    _pid = fork();
    if(_pid == 0) _launch_subprocess();
    _file_descriptor = _pipe_fd[1];
}

void ffmpeg_rtmp_pipe_out_unix::write(char *data, int size) {
    ::write(_file_descriptor, data, size);
}

ffmpeg_rtmp_pipe_out_unix::~ffmpeg_rtmp_pipe_out_unix(){
    auto prefix = get_method_prefix("ffmpeg_rtmp_pipe_out_unix.~ffmpeg_rtmp_pipe_out_unix");
    spdlog::debug("{} destructing...", prefix);
    spdlog::debug("{} Killing the PID {}...", prefix, _pid);

    int kill_result = kill(_pid, SIGTERM);
    spdlog::debug("{} kill returned {} (0 = success, -1 = error)", prefix, kill_result);

    usleep(100000);
    int wait_result = waitpid(_pid, NULL, WNOHANG);
    spdlog::debug("{} waitpid() returned {} (0 = still alive, > 0 = closed)", prefix, wait_result);
    close(_file_descriptor);
    spdlog::debug("{} ffmpeg_rtmp_pipe_out_unix was destructed!", prefix);
}