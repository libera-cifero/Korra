#include "video_codec/payload_storage.hpp"
#include "video_codec/sync_signals.hpp"
#include "video_codec/frame_codec/frame_codec.hpp"
#include <cstring>
#include <mutex>
#include <thread>

int payload_storage::payload_size(){
    return _encoder -> payload_size();
}

int payload_storage::frame_size(){
    return _encoder -> frame_size();
}

payload_storage::payload_storage(frame_codec *encoder, sync_signals *signals) {
    _encoder = encoder;
    _signals = signals;

    begin_new_payload();
    _is_frame_inited = false;
}

char *payload_storage::current_payload(){ 
    lock_guard<mutex> payloads_lock(_payloads_access);
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
    lock_guard<mutex> payloads_lock(_payloads_access);
    char *first = _payloads.front();
    _payloads.pop_front();
    if(_payloads.empty()) begin_new_payload();
    return first;
}

void payload_storage::_update_frame(){
    char *payload = _pop_payload();
    try{
        lock_guard<mutex> frame_lock(_frame_access);
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
    bool is_first_pop = false;
    if(!_is_frame_inited){
        _update_frame();
        _is_frame_inited = true;
        is_first_pop = true;
    }
    _signals->ready_response()->release();
    _signals->frame_request()->acquire();

    _frame_access.lock();
    char *result = _frame;
    _frame_access.unlock();

    thread t([&](){ _update_frame(); });
    t.detach();
    return result;
}

payload_storage::~payload_storage(){
    for(int i = 0; i < _payloads.size(); i++) delete [] _payloads[i];
    _payloads.clear();
    //delete [] _frame;
}