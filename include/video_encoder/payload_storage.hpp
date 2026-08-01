#pragma once
#include "frame_encoder/frame_encoder.hpp"
#include "sync_signals.hpp"
#include <mutex>
#include <deque>
#include <semaphore>

using namespace std;
class payload_storage {
private:
    frame_encoder *_encoder;
    sync_signals *_signals;
    
    mutex _payloads_access, _frame_access;
    deque<char*> _payloads;
    char *_frame;

    char *_pop_payload();

public:
    int payload_index;

    payload_storage(frame_encoder *encoder, sync_signals *signals);

    int payload_size();
    int frame_size();

    char *pop_frame();
    char *current_payload();

    char *begin_new_payload();
    void begin_frame_updating();
};