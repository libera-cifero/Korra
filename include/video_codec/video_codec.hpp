#pragma once
#include "data_boxer/data/korra_data.hpp"
#include "video_codec/frame_codec/frame_codec.hpp"
#include "video_codec/payload_storage.hpp"
#include "clock_generator.hpp"

class video_codec {
private:
    clock_generator *_clock_generator;   
    frame_codec *_frame_codec;
    payload_storage *_payload_storage;
public:
    video_codec(frame_codec *encoder, int fps);
    void launch();
    payload_storage *storage();
    int frame_size();
    int payload_size();

    char *decode(char *frame);
    
    ~video_codec();
};