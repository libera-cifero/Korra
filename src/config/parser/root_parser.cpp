#include "config/parser/root_parser.hpp"
#include "config/data/root_config.hpp"
#include "config/parser/encoder_parser.hpp"
#include "config/parser/pipes_parser.hpp"
#include "config/parser/video_parser.hpp"
#include "video/video_socket.hpp"

root_parser::root_parser(){
    encoder = new encoder_parser;
    pipes = new pipes_parser;
    _video = new video_parser;
}

video_listener* root_parser::parse(json j){
    video_config video_conf = _video->parse(j);
    pipes->in->video = &video_conf;
    pipes->out->video = &video_conf;
    encoder->video = &video_conf;

    
}

root_parser::~root_parser(){
    delete encoder;
    delete pipes;
    delete _video;
}