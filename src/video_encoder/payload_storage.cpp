#include "video_encoder/payload_storage.hpp"
#include "video_encoder/frame_encoder/frame_encoder.hpp"
#include <cstring>
#include <mutex>
#include <thread>

payload_storage::payload_storage(frame_encoder *encoder, binary_semaphore *ready_request, binary_semaphore *ready_response, binary_semaphore *get_frame_request) {
    _encoder = encoder;
    _ready_request = ready_request;
    _ready_response = ready_response;
    _get_frame_request = get_frame_request;
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
    return payload;
}

char *payload_storage::pop_payload(){
    lock_guard<mutex> payloads_lock(_payloads_access);
    char *first = _payloads.front();
    _payloads.pop_front();
    if(_payloads.empty()) begin_new_payload();
    return first;
}

char *payload_storage::frame(){
    _ready_request->acquire();
    if(_payloads.size() == 1) {
        char *data = pop_payload();
        _frame = _encoder -> encode(data);
    }
    _ready_response->release();
    _get_frame_request->acquire();

    lock_guard<mutex> frame_lock(_frame_access);
    char *result = _frame;
    int size = frame_size();
    _frame = new char[size];
    memset(_frame, 0, size);
    return _frame;
}

void payload_storage::begin_frame_updating(){
    std::thread thread([&]() {
        char *payload, *frame;
        {
            lock_guard<mutex> payloads_lock(_payloads_access);
            payload = pop_payload();
        }
        
        frame = _encoder -> encode(payload);
        {
            lock_guard<mutex> frame_lock(_frame_access);
            _frame = frame;
        }
    });
    thread.detach();
}