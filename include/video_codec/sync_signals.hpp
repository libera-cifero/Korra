#pragma once
#include <semaphore>

using namespace std;
class sync_signals {
private:
    binary_semaphore *_ready_request, *_ready_response, *_frame_request;
public:
    sync_signals();

    binary_semaphore *ready_request();
    binary_semaphore *ready_response();
    binary_semaphore *frame_request();

    ~sync_signals();
};