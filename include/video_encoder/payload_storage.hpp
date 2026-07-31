#pragma once
#include "frame_encoder/frame_encoder.hpp"
#include <mutex>
#include <deque>
#include <semaphore>

using namespace std;
class payload_storage {
private:
    frame_encoder *_encoder;
    binary_semaphore *_ready_request, *_ready_response, *_get_frame_request;
    
    mutex _payloads_access, _frame_access;
    deque<char*> _payloads;
    char *_frame;

    char *_pop_payload();

public:
    int payload_index;

    payload_storage(frame_encoder *encoder, binary_semaphore *ready_request, binary_semaphore *ready_response, binary_semaphore *get_frame_request);

    int payload_size();
    int frame_size();

    char *pop_frame();
    char *current_payload();

    char *begin_new_payload();
    void begin_frame_updating();
};