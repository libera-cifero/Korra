#include "config/data/video_config.hpp"
#include "config/parser/parser.hpp"
#include "pipe/in/pipe/video_pipe_in.hpp"

class video_pipe_in_parser : public abstract_parser<video_pipe_in*>, public contexted_in<video_config> {
public:
    video_pipe_in *parse(json) override;
    json serialize(video_pipe_in*) override;
};