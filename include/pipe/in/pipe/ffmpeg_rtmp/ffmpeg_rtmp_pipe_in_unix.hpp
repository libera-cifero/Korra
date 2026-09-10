#include "ffmpeg_rtmp_pipe_in.hpp"
class ffmpeg_rtmp_pipe_in_unix : public ffmpeg_rtmp_pipe_in {
private:
    int _file_descriptor;
    int _pid;
    int _pipe_fd[2];
    void _launch_subprocess();
protected:
    int __read_async() override;
public:
    ffmpeg_rtmp_pipe_in_unix(ffmpeg_rtmp_config &config);
    ~ffmpeg_rtmp_pipe_in_unix();
};