#include "config/parser/video_stream_in_parser.hpp"
#include "config/data/video_config.hpp"
#include "data_boxer/data_unboxer.hpp"
#include "pipe/in/pipe/video_pipe_in.hpp"
#include "video_codec/frame_codec/frame_codec.hpp"
#include "video_codec/video_codec.hpp"
#include "video_stream_in.hpp"
#include <stdexcept>

video_stream_in_parser::video_stream_in_parser(video_config_parser *a, frame_codec_parser *b, video_pipe_in_parser *c){
    _video_config_parser = a;
    _frame_codec_parser = b;
    _pipe_in_parser = c;
}
video_stream_in *video_stream_in_parser::parse(json j){
    video_config config = _video_config_parser->parse(j);
    _frame_codec_parser->context_in = config;
    _pipe_in_parser->context_in = config;

    frame_codec *fcodec = _frame_codec_parser->parse(j["frameCodec"]);
    video_pipe_in *pipe_in = _pipe_in_parser->parse(j["pipe"]);

    video_codec *codec = new video_codec(fcodec, config.fps);

    return new video_stream_in(config, pipe_in, codec, new data_unboxer);
}

json video_stream_in_parser::serialize(video_stream_in *stream){
    /*video_config config = stream->config();
    _frame_codec_parser->context_in = config;
    _pipe_in_parser->context_in = config;
    json root = _video_config_parser->serialize(config);
    root["frameCodec"] = _frame_codec_parser->serialize();*/

    throw runtime_error("video_stream_in_parser.serialize: serialization isn't supported!");
}
