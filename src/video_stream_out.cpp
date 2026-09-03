#include "video_stream_out.hpp"
#include "data_boxer/data/korra_data.hpp"
#include "lib/log.hpp"
#include "video_codec/payload_storage.hpp"
#include <cstdint>
#include <spdlog/spdlog.h>
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
    auto prefix = get_method_prefix("video_stream_out.run");
    spdlog::info("{} has started!", prefix);
    _is_running = true;
    _codec->launch();
    _sender_thread = thread([&](string prefix){
        payload_storage *storage = _codec->storage();
        int frame_size = storage->frame_size();
        while(_is_running){
            spdlog::info("{} frame popping...", prefix);
            uint8_t *encoded = reinterpret_cast<uint8_t*>(storage->pop_frame());
            spdlog::info("{} writing the frame to pipe...", prefix);
            _pipe->write(encoded, frame_size);
            spdlog::info("{} wrote the frame!", prefix);
        }
    },prefix);
}

void video_stream_out::write(korra_data *data){
    auto prefix = get_method_prefix("video_stream_out.write");
    spdlog::info("{} writing the data with size {}...", prefix, data->size());
    _boxer->box(data);
    spdlog::info("{} success!", prefix);
}

video_stream_out::~video_stream_out(){
    _is_running = false;
    if(_sender_thread.joinable()) _sender_thread.join();
    delete _pipe;
    delete _codec;
    delete _boxer;
}