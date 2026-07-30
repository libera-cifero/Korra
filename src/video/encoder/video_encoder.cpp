#include <stdbool.h>
#include <stdlib.h>
#include "video/encoder/video_encoder.hpp"

video_encoder::video_encoder(provider *provider, obfuscator *obfuscator){
    _provider = provider;
    _obfuscator = obfuscator;
}

int video_encoder::payload_size(){
    return _provider->payload_size();
}

int video_encoder::frame_size(){
    return _provider->frame_size();
}

char *video_encoder::encode(char *data) {
    data = _obfuscator->obfuscate(data);

    return _provider->to_frame(data);;
}

char *video_encoder::decode(char *frame) {
    char *data = _provider->to_payload(frame);
    
    return _obfuscator->deobfuscate(data);
}

video_encoder::~video_encoder(){
    delete _provider;
    delete _obfuscator;
}