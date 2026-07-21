#include "config/parser/video/pipes/pipes_parser.hpp"
#include "config/parser/video/pipes/video_pipe_in_parser.hpp"
#include "config/parser/video/pipes/video_pipe_out_parser.hpp"
#include "status_error.hpp"
#include "config/config_error.hpp"
#include "video/pipe/video_pipe_in.hpp"

template<typename T>
function<video_pipe_in_parser*(video_pipe_in*)> pipes_parser::_make_in_get_if_type(string key){
    
    return [key,this](video_pipe_in* pipe) -> video_pipe_in_parser* 
    {
        if(dynamic_cast<T*>(pipe) != nullptr) return get<0>(_parser_map[key]);
        return nullptr;
    };
}

template<typename T>
function<video_pipe_out_parser*(video_pipe_out*)> pipes_parser::_make_out_get_if_type(string key){
    return [key,this](video_pipe_out* pipe) -> video_pipe_out_parser* 
    {
        if(dynamic_cast<T*>(pipe) != nullptr) return get<1>(_parser_map[key]);
        return nullptr;
    };
}

pipes_parser::pipes_parser(){
    in = new video_pipe_in_parser;
    out = new video_pipe_out_parser;
}

string pipes_parser::_get_available_types_string(){
    string availables = "";
}

video_pipe_in_parser* pipes_parser::_get_in_parser_by_pipe(video_pipe_in *pipe) {
    return nullptr;
}

video_pipe_out_parser* pipes_parser::_get_out_parser_by_pipe(video_pipe_out *pipe) {
    return nullptr;
}



pipes_config pipes_parser::parse(json root){
    auto in_token = root["in"], out_token = root["out"], type_in_token = in_token["type"], type_out_token = out_token["type"];
    
    if(!in_token.is_object()) throw status_error("Parsing [config:pipes:in] Section \"in\" must be object!",config_error::PARSE_PIPES_IN_UNDEFINED);
    if(!out_token.is_object()) throw status_error("Parsing [config:pipes:out] Section \"out\" must be object!",config_error::PARSE_PIPES_OUT_UNDEFINED);

    if(!type_in_token.is_string()) throw status_error("Parsing [config:pipes:in:type] Type must be string!",config_error::PARSE_PIPES_IN_TYPE_NOT_STRING);
    if(!type_out_token.is_string()) throw status_error("Parsing [config:pipes:out:type] Type must be string!",config_error::PARSE_PIPES_OUT_TYPE_NOT_STRING);

    string type_in = type_in_token, type_out = type_out_token;

    
}

json pipes_parser::serialize(pipes_config config){

}

pipes_parser::~pipes_parser() {
    for(auto kvp : _parser_map) {
        auto pipes = kvp.second;
        video_pipe_in_parser *in = get<0>(pipes);
        video_pipe_out_parser *out = get<1>(pipes);

        delete in;
        delete out;
    }
}