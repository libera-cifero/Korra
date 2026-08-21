#include "config/parser/pipe/in/video_pipe_in_parser.hpp"
#include "config/data/video_config.hpp"
#include "config/parser/parser.hpp"
#include "pipe/in/pipe/video_pipe_in.hpp"

video_pipe_in *video_pipe_in_parser::parse(json j) {
    string type = j["type"];

    for(auto p : specific_parsers){ 
        if(auto x = dynamic_cast<contexted_in<video_config>*>(p)){
            x->context_in = context_in;
        }
        if(p->type() == type) return p->parse(j);
    }

    throw runtime_error(std::format("video_pipe_in_parser.parse: Can't parse video_pipe_in! Unknown type \"{}!\"", type));
}

json video_pipe_in_parser::serialize(video_pipe_in *pipe) {
    for(auto p : specific_parsers){ 
        if(auto x = dynamic_cast<contexted_in<video_config>*>(p)){
            x->context_in = context_in;
        }
        if(p->can_serialize(pipe)) return p->serialize(pipe);
    }

    throw runtime_error(std::format("video_pipe_in_parser.serialize: Can't serialize pipe!"));
}