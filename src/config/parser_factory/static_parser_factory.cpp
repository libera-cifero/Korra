#include "config/parser_factory/static_parser_factory.hpp"
#include "video/video_socket.hpp"

json_parser<video_listener*>* static_parser_factory::build(){
   /* auto parser = new root_parser();

    auto basic_pp = new basic_provider_parser;
    basic_pp -> color_codec = new color_codec_parser;
    basic_pp -> color_codec -> specific_parsers = { new rgb_palette_parser };
    parser->encoder->provider->specific_parsers = { basic_pp };

    parser->encoder->provider->length_reader->specific_parsers = { new raw_ip_length_reader_parser };
    parser->pipes->in->specific_parsers = { new ffmpeg_rtmp_pipe_in_parser };
    parser->pipes->out->specific_parsers = { new ffmpeg_rtmp_pipe_out_parser };

    return parser;*/

    return nullptr;
}