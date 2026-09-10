#pragma once
#include "frame_codec/frame_codec.hpp"
#include "payload_storage.hpp"

class video_codec {
private:
    frame_codec *_frame_codec;
    payload_storage *_payload_storage;
public:
    video_codec(frame_codec *encoder, int fps);
    void launch();
    payload_storage *storage();
    frame_codec *fcodec();
    int frame_size();
    int payload_size();

    char *decode(char *frame);
    
    ~video_codec();
};