#include "video_stream_out.hpp"
#include "data_boxer/data/korra_data.hpp"
#include "lib/log.hpp"
#include "video_codec/payload_storage.hpp"
#include <spdlog/spdlog.h>

video_stream_out::video_stream_out(video_pipe_out *pipe, video_codec *codec, data_boxer *boxer){
    _pipe = pipe;
    _codec = codec;
    _boxer = boxer;
}

void video_stream_out::send_current_frame(){
    string prefix = get_method_prefix("video_stream_out.send_current_frame");
    spdlog::debug("{} frame popping...", prefix);
    auto frame = _codec->storage()->pop_frame();
    spdlog::debug("{} frame was popped!", prefix);
    spdlog::debug("{} writing the frame to pipe_out...", prefix);
    _pipe->write(frame, _codec->frame_size());
    spdlog::debug("{} frame was writen to pipe_out!", prefix);
    delete [] frame;
    spdlog::debug("{} succsess!", prefix);
}

void video_stream_out::write(korra_data *data){
    auto prefix = get_method_prefix("video_stream_out.write");
    spdlog::debug("{} writing the data with size {}...", prefix, data->size());
    _boxer->box(data);
    spdlog::debug("{} success!", prefix);
}

video_stream_out::~video_stream_out(){
    delete _pipe;
    delete _codec;
    delete _boxer;
}