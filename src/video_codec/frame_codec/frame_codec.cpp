#include <stdbool.h>
#include <stdlib.h>
#include "video_codec/frame_codec/frame_codec.hpp"

frame_codec::frame_codec(provider *provider, cipher *cipher){
    _provider = provider;
    _cipher = cipher;
}

int frame_codec::payload_size(){
    return _provider->payload_size();
}

int frame_codec::frame_size(){
    return _provider->frame_size();
}

char *frame_codec::encode(char *data) {
    char *encryptd = _cipher->encrypt(data);
    char *frame = _provider->to_frame(encryptd);
    delete [] encryptd;
    return frame;
}

char *frame_codec::decode(char *frame) {
    char *encryptd = _provider->to_payload(frame);
    char *data = _cipher->decrypt(encryptd);
    delete [] encryptd;
    return data;
}

frame_codec::~frame_codec(){
    delete _provider;
    delete _cipher;
}