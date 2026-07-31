#include <stdbool.h>
#include <stdlib.h>
#include "video_encoder/frame_encoder/frame_encoder.hpp"

frame_encoder::frame_encoder(provider *provider, obfuscator *obfuscator){
    _provider = provider;
    _obfuscator = obfuscator;
}

int frame_encoder::payload_size(){
    return _provider->payload_size();
}

int frame_encoder::frame_size(){
    return _provider->frame_size();
}

char *frame_encoder::encode(char *data) {
    data = _obfuscator->obfuscate(data);

    return _provider->to_frame(data);;
}

char *frame_encoder::decode(char *frame) {
    char *data = _provider->to_payload(frame);
    
    return _obfuscator->deobfuscate(data);
}

frame_encoder::~frame_encoder(){
    delete _provider;
    delete _obfuscator;
}