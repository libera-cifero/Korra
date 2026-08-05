#pragma once

#include "config/data/block_codec_config.hpp"
#include "config/data/mosaic_config.hpp"
#include "config/parser/parser.hpp"
#include "video_codec/frame_codec/provider/mosaic/block_codec/block_codec.hpp"

class block_codec_parser : public abstract_parser<block_codec*>, public contexted_in<mosaic_config>, public contexted_out<block_codec_config>
{
private:
    void _update_context_out(int bits_per_number);
    void _update_parser_context_in(specific_parser<block_codec*> *parser);
public:
    block_codec* parse(json) override;
    json serialize(block_codec*) override;
};