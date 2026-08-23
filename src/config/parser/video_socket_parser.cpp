#include "config/parser/video_socket_parser.hpp"
#include "config/parser/parser.hpp"
#include "config/parser/tun_parser.hpp"
#include "config/parser/video_config_parser.hpp"
#include "video_socket.hpp"

video_socket_parser::video_socket_parser(frame_codec_parser *codec, video_pipe_in_parser *pipe_in, video_pipe_out_parser *pipe_out) {
    _frame_codec_parser = codec;
    _pipe_in_parser = pipe_in;
    _pipe_out_parser = pipe_out;
    _tun_parser = new tun_parser;
    _video_config_parser = new video_config_parser;

    _stream_in_parser = new video_stream_in_parser(_video_config_parser, _frame_codec_parser, _pipe_in_parser);
    _stream_out_parser = new video_stream_out_parser(_video_config_parser, _frame_codec_parser, _pipe_out_parser);
}

video_socket *video_socket_parser::parse(json j){
    video_socket_settings settings;
    settings.tunnel = _tun_parser->parse(j["tun"]);
    settings.stream_in = _stream_in_parser->parse(j["in"]);
    settings.stream_out = _stream_out_parser->parse(j["out"]);

    return new video_socket(settings);
}

json video_socket_parser::serialize(video_socket *socket){
    json root = json::object();
    root["tun"] = _tun_parser -> serialize(socket->get_tun());
    root["in"] = _stream_in_parser -> serialize(socket->stream_in());
    root["out"] = _stream_out_parser -> serialize(socket->stream_out());

    throw root;
}

video_socket_parser::~video_socket_parser(){
    delete _tun_parser;
    delete _video_config_parser;
    delete _pipe_in_parser;
    delete _pipe_out_parser;
    delete _frame_codec_parser;

    delete _stream_in_parser;
    delete _stream_out_parser;
}