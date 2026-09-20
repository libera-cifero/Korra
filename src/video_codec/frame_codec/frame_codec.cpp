#include <spdlog/spdlog.h>
#include <stdbool.h>
#include <stdlib.h>
#include "video_codec/frame_codec/frame_codec.hpp"
#include "lib/log.hpp"
#include "video_codec/frame_codec/provider/provider.hpp"

frame_codec::frame_codec(provider *provider, cipher *cipher){
    _provider = provider;
    _cipher = cipher;
}

provider *frame_codec::get_provider(){
    return _provider;
}

cipher *frame_codec::get_cipher(){
    return _cipher;
}

int frame_codec::payload_size(){
    return _provider->payload_size() - _cipher->header_size();
}

int frame_codec::frame_size(){
    return _provider->frame_size();
}

char *frame_codec::encode(char *data) {
    auto prefix = get_method_prefix("frame_codec.encode");
    spdlog::trace("{} encoding...", prefix);
    char *encrypted = _cipher->encrypt(data);
    char *frame = _provider->to_frame(encrypted);
    delete [] encrypted;
    spdlog::trace("{} encoding completed!", prefix);

    return frame;
}

bool frame_codec::_is_void_payload(char *payload){
    int size = payload_size();
    for(int i = 0; i < size; i++) if(payload[i] != 0) return false;
    return true;
}

char *frame_codec::decode(char *frame) {
    auto prefix = get_method_prefix("frame_codec.decode");
    spdlog::trace("{} decoding...", prefix);
    char *encrypted = _provider->to_payload(frame);
    if(_is_void_payload(encrypted)) {
        delete [] encrypted;
        return nullptr;
    }
    char *data = _cipher->decrypt(encrypted);
    delete [] encrypted;
    spdlog::trace("{} decoding completed!", prefix);
    return data;
}

frame_codec::~frame_codec(){
    string prefix = get_method_prefix("frame_codec.~frame_codec");
    spdlog::debug("{} destructing...", prefix);
    delete _provider;
    delete _cipher;
    spdlog::debug("{} destructed...", prefix);
}