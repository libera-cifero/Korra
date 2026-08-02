#include "video_encoder/payload_storage.hpp"
#include "video_encoder/sync_signals.hpp"
#include "video_encoder/frame_encoder/frame_encoder.hpp"
#include <cstring>
#include <mutex>
#include <thread>

int payload_storage::payload_size(){
    return _encoder -> payload_size();
}

int payload_storage::frame_size(){
    return _encoder -> frame_size();
}

payload_storage::payload_storage(frame_encoder *encoder, sync_signals *signals) {
    _encoder = encoder;
    _signals = signals;
    _frame = new char[payload_size()];
    memset(_frame, 0, payload_size());

    begin_new_payload();
    _is_frame_inited = false;
}

char *payload_storage::current_payload(){ 
    return _payloads.back(); 
}

char *payload_storage::begin_new_payload(){
    int size = payload_size();
    char *payload = new char[size];
    memset(payload, 0, size);
    _payloads.push_back(payload);
    payload_index = 2;//first 2 bytes of buffer are used for data_count encounting
    return payload;
}

char *payload_storage::_pop_payload(){
    char *first = _payloads.front();
    _payloads.pop_front();
    if(_payloads.empty()) begin_new_payload();
    return first;
}

void payload_storage::_update_frame(){
    char *payload = _pop_payload();
    try{
        _frame = _encoder -> encode(payload);
    }
    catch(...) { 
        delete [] payload;
        throw;
    }

    delete [] payload;
}

char *payload_storage::pop_frame(){
    _signals->ready_request()->acquire();
    if(!_is_frame_inited){
        _update_frame();
        _is_frame_inited = true;
    }
    _signals->ready_response()->release();
    _signals->frame_request()->acquire();

    char *result = _frame;

    if(_payloads.size() > 1 || payload_index == payload_size() - 1){
        _update_frame();
    }
    return result;
}

