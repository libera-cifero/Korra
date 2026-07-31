#include <stdbool.h>
#include <stdlib.h>
#include "video_encoder/frame_encoder/frame_encoder.hpp"

frame_encoder::frame_encoder(provider *provider, cipher *cipher){
    _provider = provider;
    _cipher = cipher;
}

int frame_encoder::payload_size(){
    return _provider->payload_size();
}

int frame_encoder::frame_size(){
    return _provider->frame_size();
}

char *frame_encoder::encode(char *data) {
    char *encryptd = _cipher->encrypt(data);
    char *frame = _provider->to_frame(encryptd);
    delete [] encryptd;
    return frame;
}

char *frame_encoder::decode(char *frame) {
    char *encryptd = _provider->to_payload(frame);
    char *data = _cipher->decrypt(encryptd);
    delete [] encryptd;
    return data;
}

frame_encoder::~frame_encoder(){
    delete _provider;
    delete _cipher;
}