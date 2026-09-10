#include "video_codec/video_codec.hpp"
#include "lib/log.hpp"
#include <spdlog/spdlog.h>

video_codec::video_codec(frame_codec *encoder, int fps){
    _frame_codec = encoder;
    _payload_storage = new payload_storage(encoder);
}

frame_codec *video_codec::fcodec(){
    return _frame_codec;
}

int video_codec::frame_size(){
    return _frame_codec->frame_size();
}

int video_codec::payload_size(){
    return _payload_storage->payload_size();
}

payload_storage *video_codec::storage() { return _payload_storage; }

char *video_codec::decode(char *frame){
    return _frame_codec->decode(frame);
}

video_codec::~video_codec(){
    auto prefix = get_method_prefix("video_codec.~video_codec");
    spdlog::debug("{} destructing...", prefix);
    delete _payload_storage;
    delete _frame_codec;
    spdlog::debug("{} video_codec was destructed!", prefix);
}