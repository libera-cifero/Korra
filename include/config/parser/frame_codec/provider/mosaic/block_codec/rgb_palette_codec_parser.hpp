#include "config/data/block_codec_config.hpp"
#include "config/parser/parser.hpp"
#include "video_codec/frame_codec/provider/mosaic/block_codec/block_codec.hpp"

class rgb_palette_codec_parser : public specific_parser<block_codec*>, public contexted_in<block_codec_config> {
public:
    block_codec* parse(json) override;
    json serialize(block_codec*) override;  
    string type() override;
    bool can_serialize(block_codec*) override;
};