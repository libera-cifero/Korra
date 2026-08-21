#include "config/parser/pipe/out/video_pipe_out_parser.hpp"

video_pipe_out *video_pipe_out_parser::parse(json j) {
    string type = j["type"];

    for(auto p : specific_parsers) if(p->type() == type) return p->parse(j[type + "Settings"]);

    throw runtime_error(std::format("video_pipe_out_parser.parse: Can't parse video_pipe_out! Unknown type \"{}!\"", type));
}

json video_pipe_out_parser::serialize(video_pipe_out *pipe) {
    for(auto p : specific_parsers){
        
        if(p->can_serialize(pipe)) return p->serialize(pipe);
    }
       
    throw runtime_error(std::format("video_pipe_out_parser.serialize: Can't serialize pipe!"));
}