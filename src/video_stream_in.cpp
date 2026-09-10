#include "video_stream_in.hpp"
#include "config/data/video_config.hpp"
#include "data_boxer/data/korra_data.hpp"
#include "lib/log.hpp"
#include <spdlog/spdlog.h>
#include <vector>

video_stream_in::video_stream_in(video_config &config, video_pipe_in *pipe, video_codec *codec, data_unboxer *unboxer)
{
    _pipe = pipe;
    _codec = codec;
    _unboxer = unboxer;
    _config = config;
}

video_config video_stream_in::config() { return _config; }

char *video_stream_in::read_frame(){
    return _pipe->read(_codec->frame_size());
}

void video_stream_in::unbox_frame(char *frame, vector<korra_data*> &buffer) {
    auto prefix = get_method_prefix("video_stream_in.unbox_frame");
    char *payload = _codec->decode(reinterpret_cast<char*>(frame));
    if(payload != nullptr){
        spdlog::info("{} unboxing payload...", prefix);
        _unboxer->put_payload(payload, _codec->payload_size());
        korra_data *data = nullptr;
        do{
            data = _unboxer->unbox();
            buffer.push_back(data);
            spdlog::info("{} korra_data unboxed!", prefix);
        }
        while(data != nullptr);
        delete [] payload;
    }

    spdlog::info("{} success!", prefix);
}

video_stream_in::~video_stream_in(){
    auto prefix = get_method_prefix("video_stream_in.~video_stream_in");
    spdlog::debug("{} destructing...", prefix);
    delete _pipe;
    delete _codec;
    delete _unboxer;
    spdlog::debug("{} video_stream_in was destructed!", prefix);
}