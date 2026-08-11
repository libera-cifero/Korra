#pragma once
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
    ~video_codec();
};