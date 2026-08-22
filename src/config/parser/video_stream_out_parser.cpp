#include "config/parser/video_stream_out_parser.hpp"
#include "config/data/video_config.hpp"
#include "data_boxer/data_boxer.hpp"
#include "pipe/out/pipe/video_pipe_out.hpp"
#include "video_codec/frame_codec/frame_codec.hpp"
#include "video_codec/video_codec.hpp"
#include "video_stream_out.hpp"
#include <stdexcept>

video_stream_out_parser::video_stream_out_parser(video_config_parser *a,frame_codec_parser *b, video_pipe_out_parser *c){
    _video_config_parser = a;
    _frame_codec_parser = b;
    _pipe_out_parser = c;
}

video_stream_out *video_stream_out_parser::parse(json j){
    video_config config = _video_config_parser->parse(j);
    _frame_codec_parser->context_in = config;
    _pipe_out_parser->context_in = config;

    frame_codec *fcodec = _frame_codec_parser->parse(j["frameCodec"]);
    video_pipe_out *pipe_out = _pipe_out_parser->parse(j["pipe"]);

    video_codec *codec = new video_codec(fcodec, config.fps);

    return new video_stream_out(pipe_out, codec, new data_boxer(codec->storage()));
}

json video_stream_out_parser::serialize(video_stream_out *stream){
    //sorry, but I tired to write a templated code for parsers
    throw runtime_error("video_stream_out_parser.serialize: serialization isn't supported!");
}