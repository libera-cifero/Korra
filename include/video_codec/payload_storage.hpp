#pragma once
#include "frame_codec/frame_codec.hpp"
#include "sync_signals.hpp"
#include <mutex>
#include <deque>

using namespace std;
class payload_storage {
private:
    frame_codec *_encoder;
    sync_signals *_signals;
    
    mutex _payloads_access, _frame_access;
    deque<char*> _payloads;
    char *_frame;
    bool _is_frame_inited;

    char *_pop_payload();
    void _update_frame();
public:
    int payload_index;

    payload_storage(frame_codec *encoder, sync_signals *signals);

    int payload_size();
    int frame_size();
    char *current_payload();

    char *pop_frame();
    char *begin_new_payload();

    ~payload_storage();
};