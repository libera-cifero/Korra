#include "config/parser/frame_codec/provider/mosaic/block_codec/block_codec_parser.hpp"
#include "config/data/block_codec_config.hpp"
#include "config/data/mosaic_config.hpp"
#include "config/data/video_config.hpp"
#include "config/parser/parser.hpp"
#include "video_codec/frame_codec/provider/mosaic/block_codec/block_codec.hpp"
#include <cstring>
#include <format>
#include <stdexcept>

void block_codec_parser::_update_context_out(int bits_per_number){
    memcpy((mosaic_config*)&context_out, &context_in, sizeof(mosaic_config));
    context_out.bits_per_number = bits_per_number;
}

void block_codec_parser::_update_parser_context_in(specific_parser<block_codec*> *parser){
    if(auto in = dynamic_cast<contexted_in<block_codec_config>*>(parser)) in->context_in = context_out;
    else if(auto in = dynamic_cast<contexted_in<mosaic_config>*>(parser)) in->context_in = context_out;
    else if(auto in = dynamic_cast<contexted_in<video_config>*>(parser)) in->context_in = context_out;
}

block_codec *block_codec_parser::parse(json j){
    string t = j["type"];
    _update_context_out(j["bitsPerNumber"]);
    for(auto p : specific_parsers){
        string type = p->type();
        if(type == t){
            _update_parser_context_in(p);
            return p -> parse(j[type + "Settings"]);
        }
    }

    throw runtime_error(std::format("Can't parse block_codec! Unknown type \"{}\"!", t));
}

json block_codec_parser::serialize(block_codec *codec) {
    int bits_per_number = codec->bits_per_number();
    _update_context_out(bits_per_number);
    for(auto parser : specific_parsers){
        if(parser -> can_serialize(codec)){
            _update_parser_context_in(parser);
            json j = json::object({{"bitsPerNumber", bits_per_number}});
            j[parser->type() + "Settings"] = parser->serialize(codec);
            return j;
        }
    }

    throw runtime_error("Can't serialize block_codec!");
}