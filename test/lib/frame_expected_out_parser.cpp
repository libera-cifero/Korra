#include "frame_expected_out_parser.hpp"
#include "config/data/video_config.hpp"
#include "config/parser/frame_codec/frame_codec_parser.hpp"
#include "config/parser/parser.hpp"
#include "config/parser/video_config_parser.hpp"
#include "frame_meta.hpp"
#include "video_codec/frame_codec/frame_codec.hpp"
#include <cstring>

frame_expected_out_parser::frame_expected_out_parser(json_parser<frame_codec*> *parser){
    _codec_parser = (frame_codec_parser*)parser;
    _config_parser = new video_config_parser;
}

frame_expected_out frame_expected_out_parser::parse(json object){
    frame_expected_out result;
    video_config config = _config_parser->parse(object);
    _codec_parser->context_in = config;
    result.codec = _codec_parser->parse(object["frameCodec"]);

    result.data.path = object["data"]["path"];
    result.data.payload = object["data"]["payload"].get<vector<char>>();

    memcpy((video_config*)&result, &config, sizeof(video_config));
    return result;
}

json frame_expected_out_parser::serialize(frame_expected_out value){
    json object = _config_parser->serialize((video_config)value);
    
    object["frameCodec"] = _codec_parser->serialize(value.codec);
    object["data"] = json::object();
    object["data"]["path"] = value.data.path;
    object["data"]["payload"] = value.data.payload;

    return object;
}

frame_codec_parser *frame_expected_out_parser::codec_parser(){
    return _codec_parser;
}

video_config_parser *frame_expected_out_parser::config_parser(){
    return _config_parser;
}

frame_expected_out_parser::~frame_expected_out_parser(){
    delete _codec_parser;
    delete _config_parser;
}