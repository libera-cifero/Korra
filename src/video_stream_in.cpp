#include "video_stream_in.hpp"
#include "config/data/video_config.hpp"
#include "data_boxer/data/korra_data.hpp"
#include <cstdint>

video_stream_in::video_stream_in(video_config &config, video_pipe_in *pipe, video_codec *codec, data_unboxer *unboxer)
{
    _pipe = pipe;
    _codec = codec;
    _unboxer = unboxer;
    _config = config;
}

video_config video_stream_in::config() { return _config; }

void video_stream_in::read(vector<korra_data*> &buffer) {
    uint8_t *frame = _pipe->read(_codec->frame_size());
    char *payload = _codec->decode(reinterpret_cast<char*>(frame));
    if(payload != nullptr){
        _unboxer->put_payload(payload, _codec->payload_size());
        korra_data *data = nullptr;
        do{
            data = _unboxer->unbox();
            buffer.push_back(data);
        }
        while(data != nullptr);
    }
}

video_stream_in::~video_stream_in(){
    delete _pipe;
    delete _codec;
    delete _unboxer;
}