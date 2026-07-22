#include <cstring>
#include <stdbool.h>
#include <stdlib.h>
#include "video/encoder/video_encoder.hpp"

video_encoder::video_encoder(video_encoder_config *config){
    _config = config;
    
}

vector<video_frame>* video_encoder::encode_bytes(void *bytes, uint32_t bytes_size) {
    int length;
    char *compressed_data = _config->compressor->compress((char*)bytes, &length);
    char *corrected_data = _config->corrector->make_recoverable(compressed_data, &length);
    
}

void* video_encoder::decode_bytes(vector<video_frame> &frames, uint32_t &bytes_size) {

}


video_encoder::~video_encoder(){
    delete _config->provider;
    delete _config->corrector;
    delete _config->compressor;
    delete _config->obfuscator;
    delete _config;
}