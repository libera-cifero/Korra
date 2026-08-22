#include "video_stream_out.hpp"
#include "data_boxer/data/korra_data.hpp"
#include "video_codec/payload_storage.hpp"
#include <cstdint>
#include <thread>

video_stream_out::video_stream_out(video_pipe_out *pipe, video_codec *codec, data_boxer *boxer){
    _pipe = pipe;
    _codec = codec;
    _boxer = boxer;
}

bool video_stream_out::is_running(){
    return _is_running;
}

void video_stream_out::run(){
    _is_running = true;
    _codec->launch();
    _sender_thread = thread([&](){
        payload_storage *storage = _codec->storage();
        while(_is_running){
            uint8_t *encoded = reinterpret_cast<uint8_t*>(storage->pop_frame());
            _pipe->write(encoded, storage->frame_size());
        }
    });
}

void video_stream_out::write(korra_data *data){
    _boxer->box(data);
}

video_stream_out::~video_stream_out(){
    _is_running = false;
    if(_sender_thread.joinable()) _sender_thread.join();
    delete _pipe;
    delete _codec;
    delete _boxer;
}