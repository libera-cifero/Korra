#include "../video_pipe_out_parser.hpp"

class ffmpeg_rtmp_pipe_out_parser : public video_pipe_out_parser {
public:
    video_pipe_out* parse(json) override;
    json serialize(video_pipe_out*) override;
};