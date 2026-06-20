#include <stdbool.h>
#include <stdlib.h>
#include "encoder/video_encoder.hpp"
#include "color.h"

video_encoder::video_encoder(video_encoder_config config){
    _config = config;
    
}

video_encoder::~video_encoder(){
    delete _config._provider;
    delete _config._corrector;
    delete _config._compressor;
    delete _config._obfuscator;
}