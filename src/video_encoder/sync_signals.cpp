#include "video_encoder/sync_signals.hpp"
#include <semaphore>

sync_signals::sync_signals(){
    _ready_request = new binary_semaphore(0);
    _ready_response = new binary_semaphore(0);
    _frame_request = new binary_semaphore(0);
}

binary_semaphore *sync_signals::ready_request() {
    return _ready_request;
}

binary_semaphore *sync_signals::ready_response() {
    return _ready_response;
}

binary_semaphore *sync_signals::frame_request() {
    return _frame_request;
}

sync_signals::~sync_signals(){
    delete _ready_request;
    delete _ready_response;
    delete _frame_request;
}