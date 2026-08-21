#include "config/parser/parser.hpp"
#include "pipe/in/pipe/video_pipe_in.hpp"
#include "config/data/video_config.hpp"

class ffmpeg_rtmp_pipe_in_parser : public specific_parser<video_pipe_in*>, public contexted_in<video_config> {
public:
    string type() override;
    bool can_serialize(video_pipe_in *pipe) override;
    video_pipe_in *parse(json) override;
    json serialize(video_pipe_in*) override;
};