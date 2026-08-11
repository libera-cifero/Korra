#include "video_codec/sync_signals.hpp"
#include <semaphore>

sync_signals::sync_signals(){
    _ready_request = new binary_semaphore(0);
}

binary_semaphore *sync_signals::ready_request() {
    return _ready_request;
}

sync_signals::~sync_signals(){
    delete _ready_request;
}