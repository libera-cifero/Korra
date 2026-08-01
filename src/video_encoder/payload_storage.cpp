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

char *payload_storage::begin_new_payload(){
    int size = payload_size();
    char *payload = new char[size];
    memset(payload, 0, size);
    _payloads.push_back(payload);
    payload_index = 2;//first 2 bytes of buffer are used for data_count encounting
    return payload;
}

payload_storage::payload_storage(frame_encoder *encoder, sync_signals *signals) {
    _encoder = encoder;
    _signals = signals;

    begin_new_payload();
}

char *payload_storage::current_payload(){ 
    lock_guard<mutex> payloads_lock(_payloads_access);
    return _payloads.back(); 
}

char *payload_storage::_pop_payload(){
    lock_guard<mutex> payloads_lock(_payloads_access);
    char *first = _payloads.front();
    _payloads.pop_front();
    if(_payloads.empty()) begin_new_payload();
    return first;
}

char *payload_storage::pop_frame(){
    _signals->ready_request()->acquire();
    if(_payloads.size() == 1) {
        char *data = _pop_payload();
        lock_guard<mutex> frame_lock(_frame_access);
        _frame = _encoder -> encode(data);
    }
    _signals->ready_response()->release();
    _signals->frame_request()->acquire();

    _frame_access.lock();
    char *result = _frame;
    _frame_access.unlock();

    if(_payloads.size() > 1) begin_frame_updating();
    return result;
}

void payload_storage::begin_frame_updating(){
    std::thread thread([&]() {
        char *payload = payload = _pop_payload();
        try{
            lock_guard<mutex> frame_lock(_frame_access);
            _frame = _encoder -> encode(payload);
        }
        catch(...) { 
            delete [] payload;
            throw;
        }

        delete [] payload;
    });
    thread.detach();
}