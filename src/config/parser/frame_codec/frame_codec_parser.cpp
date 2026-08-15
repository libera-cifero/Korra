#include "config/parser/frame_codec/frame_codec_parser.hpp"
#include "config/data/video_config.hpp"
#include "video_codec/frame_codec/cipher/cipher.hpp"
#include "video_codec/frame_codec/frame_codec.hpp"
#include "video_codec/frame_codec/provider/provider.hpp"
#include <cstring>

frame_codec_parser::frame_codec_parser(provider_parser *p, cipher_parser *c) {
    _provider_parser = p;
    _cipher_parser = c;
}

void frame_codec_parser::_update_cipher_config(int encrypted_size){
    memcpy((video_config*)&_cipher_parser->context_in, &context_in, sizeof(video_config));
    _cipher_parser->context_in->encrypted_size = encrypted_size;
}

frame_codec* frame_codec_parser::parse(json obj){
    _provider_parser->context_in = context_in;
    provider *p = _provider_parser->parse(obj["provider"]);
    _update_cipher_config(p->payload_size());
    cipher *c = _cipher_parser->parse(obj["cipher"]);

    return new frame_codec(p,c);
}

json frame_codec_parser::serialize(frame_codec *codec){
    _provider_parser->context_in = context_in;
    cipher *ciph = codec->get_cipher();
    _update_cipher_config(ciph->encrypted_size());
    json obj = json::object();
    obj["provider"] = _provider_parser->serialize(codec->get_provider());
    obj["cipher"] = _cipher_parser->serialize(ciph);

    return obj;
}

frame_codec_parser::~frame_codec_parser(){
    delete _provider_parser;
    delete _cipher_parser;
}