#include "video_pipe/video_pipe_out.hpp"
#include "config/parser/parser.hpp"

class video_pipe_out_parser : public json_parser<video_pipe_out*>{
public:
    virtual video_pipe_out* parse(json) override;
    virtual json serialize(video_pipe_out*) override;
};