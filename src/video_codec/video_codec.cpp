#include "video_codec/video_codec.hpp"
#include "video_codec/payload_storage.hpp"
#include <cmath>

video_codec::video_codec(frame_codec *encoder, int fps){
    _frame_codec = encoder;
    _clock_generator = new clock_generator((int)round(1000.0f / fps));
    _payload_storage = new payload_storage(encoder, _clock_generator->signals());
}

void video_codec::launch(){
    _clock_generator->launch();
}

payload_storage *video_codec::storage() { return _payload_storage; }

video_codec::~video_codec(){
    delete _payload_storage;
    delete _frame_codec;
    delete _clock_generator;
}