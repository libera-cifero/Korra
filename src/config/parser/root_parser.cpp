#include "config/parser/root_parser.hpp"
#include "config/data/root_config.hpp"
#include "config/parser/encoder_parser.hpp"
#include "config/parser/pipes_parser.hpp"
#include "config/parser/video_parser.hpp"

root_parser::root_parser(){
    encoder = new encoder_parser;
    pipes = new pipes_parser;
    _video = new video_parser;
}

root_config root_parser::parse(json j){
    video_config video_conf = _video->parse(j);
    
}

root_parser::~root_parser(){
    delete encoder;
    delete pipes;
    delete _video;
}