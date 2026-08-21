#include "config/data/video_config.hpp"
#include "config/parser/parser.hpp"
#include "pipe/out/pipe/video_pipe_out.hpp"

class video_pipe_out_parser : public abstract_parser<video_pipe_out*>, public contexted_in<video_config> {
public:
    video_pipe_out *parse(json) override;
    json serialize(video_pipe_out*) override;
};