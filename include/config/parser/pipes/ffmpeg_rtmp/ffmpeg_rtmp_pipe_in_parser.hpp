#include "../video_pipe_in_parser.hpp"

class ffmpeg_rtmp_pipe_in_parser : public video_pipe_in_parser {
public:
    video_pipe_in* parse(json) override;
    json serialize(video_pipe_in*) override;
};