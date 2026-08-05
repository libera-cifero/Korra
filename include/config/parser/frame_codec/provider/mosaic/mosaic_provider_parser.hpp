#pragma once

#include "config/data/mosaic_config.hpp"
#include "config/data/video_config.hpp"
#include "config/parser/frame_codec/provider/mosaic/block_codec/block_codec_parser.hpp"
#include "config/parser/parser.hpp"
#include "video_codec/frame_codec/provider/provider.hpp"
#include <string>

class mosaic_provider_parser : public specific_parser<provider*>, public contexted_in<video_config>, public contexted_out<mosaic_config>
{
private:
    block_codec_parser *_block_codec_parser;
    void _update_context_out(int block_size);
public:
    mosaic_provider_parser(block_codec_parser *);
    provider* parse(json) override;
    json serialize(provider*) override;  
    string type() override;
    bool can_serialize(provider*) override;

    ~mosaic_provider_parser();
};