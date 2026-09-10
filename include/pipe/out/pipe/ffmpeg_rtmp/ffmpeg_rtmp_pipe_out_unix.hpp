#include "config/data/ffmpeg_rtmp_config.hpp"
#include "ffmpeg_rtmp_pipe_out.hpp"

class ffmpeg_rtmp_pipe_out_unix : public ffmpeg_rtmp_pipe_out{
private:
    int _pid;
    int _pipe_fd[2];
    int _file_descriptor;
    void _launch_subprocess();
public:
    ffmpeg_rtmp_pipe_out_unix(ffmpeg_rtmp_config &config);
    void write(char *data, int size) override;
    ~ffmpeg_rtmp_pipe_out_unix();
};