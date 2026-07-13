#include "video_pipe/video_pipe_in.hpp"
#include "config/parser/parser.hpp"

class video_pipe_in_parser : public json_parser<video_pipe_in*>{
public:
    virtual video_pipe_in* parse(json) override;
    virtual json serialize(video_pipe_in*) override;
};