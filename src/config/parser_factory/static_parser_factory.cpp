#include "config/parser_factory/static_parser_factory.hpp"
#include "config/parser/frame_codec/frame_codec_parser.hpp"
#include "config/parser/frame_codec/provider/mosaic/block_codec/block_codec_parser.hpp"
#include "config/parser/frame_codec/provider/mosaic/mosaic_provider_parser.hpp"
#include "config/parser/frame_codec/provider/provider_parser.hpp"
#include "config/parser/frame_codec/provider/mosaic/block_codec/rgb_palette_codec_parser.hpp"
#include "config/parser/frame_codec/cipher/cipher_parser.hpp"
#include "config/parser/frame_codec/cipher/none_cipher_parser.hpp"
#include "config/parser/frame_codec/cipher/xchacha20_cipher_parser.hpp"
#include "config/parser/pipe/in/video_pipe_in_parser.hpp"
#include "config/parser/pipe/in/ffmpeg_rtmp_pipe_in_parser.hpp"
#include "config/parser/pipe/out/video_pipe_out_parser.hpp"
#include "config/parser/pipe/out/ffmpeg_rtmp_pipe_out_parser.hpp"
#include "config/parser/video_socket_parser.hpp"

#include "video_socket.hpp"

json_parser<video_socket*>* static_parser_factory::build(){
    //building frame_codec_parser
    provider_parser *p = new provider_parser;
    block_codec_parser *block = new block_codec_parser;
    block->specific_parsers = { new rgb_palette_codec_parser };
    p->specific_parsers = { new mosaic_provider_parser(block) };

    cipher_parser *c = new cipher_parser;
    c->specific_parsers = { new none_cipher_parser, new xchacha20_cipher_parser };

    frame_codec_parser *codec = new frame_codec_parser(p, c);

    //building video pipe parsers
    video_pipe_in_parser *pipe_in = new video_pipe_in_parser;
    pipe_in->specific_parsers = { new ffmpeg_rtmp_pipe_in_parser };

    video_pipe_out_parser *pipe_out = new video_pipe_out_parser;
    pipe_out->specific_parsers = { new ffmpeg_rtmp_pipe_out_parser };


    return new video_socket_parser(codec, pipe_in, pipe_out);
}