#include <stdbool.h>
#include <stdlib.h>
#include "video/encoder/video_encoder.hpp"

video_encoder::video_encoder(video_encoder_config *config){
    _config = config;
    
}

video_encoder::~video_encoder(){
    delete _config->provider;
    delete _config->corrector;
    delete _config->compressor;
    delete _config->obfuscator;
    delete _config;
}