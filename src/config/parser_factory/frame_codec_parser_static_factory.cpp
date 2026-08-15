#include "config/parser_factory/frame_codec_parser_static_factory.hpp"
#include "config/parser/parser.hpp"
#include "video_codec/frame_codec/frame_codec.hpp"
#include "config/parser/frame_codec/frame_codec_parser.hpp"
#include "config/parser/frame_codec/cipher/none_cipher_parser.hpp"
#include "config/parser/frame_codec/cipher/xchacha20_cipher_parser.hpp"
#include "config/parser/frame_codec/provider/mosaic/mosaic_provider_parser.hpp"
#include "config/parser/frame_codec/provider/mosaic/block_codec/rgb_palette_codec_parser.hpp"


json_parser<frame_codec*> *frame_codec_parser_static_factory::build() {
    provider_parser *p = new provider_parser;
    block_codec_parser *block = new block_codec_parser;
    block->specific_parsers = { new rgb_palette_codec_parser };
    mosaic_provider_parser *mosaic = new mosaic_provider_parser(block);
    p->specific_parsers = { mosaic };
    cipher_parser *c = new cipher_parser;
    c->specific_parsers = { new none_cipher_parser, new xchacha20_cipher_parser };
    frame_codec_parser *parser = new frame_codec_parser(p, c);
    return parser;
}