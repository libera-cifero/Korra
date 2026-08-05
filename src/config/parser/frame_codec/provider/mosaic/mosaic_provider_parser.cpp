#include "config/parser/frame_codec/provider/mosaic/mosaic_provider_parser.hpp"
#include "config/data/mosaic_config.hpp"
#include "config/data/video_config.hpp"
#include "config/parser/frame_codec/provider/mosaic/block_codec/block_codec_parser.hpp"
#include "video_codec/frame_codec/provider/mosaic/mosaic_settings.hpp"
#include "video_codec/frame_codec/provider/mosaic_provider.hpp"
#include "video_codec/frame_codec/provider/provider.hpp"
#include <cstring>

using mpp = mosaic_provider_parser;

mpp::mosaic_provider_parser(block_codec_parser *parser){
    _block_codec_parser = parser;
}

string mpp::type(){
    return "mosaic";
}

bool mpp::can_serialize(provider *p){
    return dynamic_cast<mosaic_provider*>(p) != nullptr;
}

void mpp::_update_context_out(int block_size){
    memcpy((video_config*)&context_out, &context_in, sizeof(video_config));
    context_out.block_size = block_size;
}

provider *mpp::parse(json j){
    _update_context_out(j["blockSize"]);
    _block_codec_parser -> context_in = context_out;
    mosaic_settings *settings = new mosaic_settings;
    memcpy((mosaic_config*)settings, &context_out, sizeof(mosaic_config));
    settings->codec = _block_codec_parser->parse(j["blockCodec"]);
    return new mosaic_provider(settings); 
}

json mpp::serialize(provider *p){
    mosaic_provider *p1=static_cast<mosaic_provider*>(p); 
    auto settings = p1->settings();
    int block_size = settings -> block_size;
    _update_context_out(block_size);
    _block_codec_parser -> context_in = context_out;
    json j = json::object({{"blockSize", block_size}});
    j["blockCodec"] = _block_codec_parser -> serialize(settings -> codec);
    return j;
}

mpp::~mosaic_provider_parser(){
    delete _block_codec_parser;
}