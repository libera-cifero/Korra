#include "video_encoder/video_encoder.hpp"
#include "video_encoder/payload_storage.hpp"
#include <cmath>

video_encoder::video_encoder(frame_encoder *encoder, int fps, int gap_timeout){
    _frame_encoder = encoder;
    _clock_generator = new clock_generator((int)round(1000.0f / fps), gap_timeout);
    _payload_storage = new payload_storage(encoder, _clock_generator->ready_request(), _clock_generator->ready_response(), _clock_generator->frame_request());
}

void video_encoder::launch(){
    _clock_generator->launch();
}

payload_storage *video_encoder::storage() { return _payload_storage; }

video_encoder::~video_encoder(){
    delete _payload_storage;
    delete _frame_encoder;
    delete _clock_generator;
}