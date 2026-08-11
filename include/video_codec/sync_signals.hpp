#pragma once
#include <semaphore>

using namespace std;
class sync_signals {
private:
    binary_semaphore *_ready_request;
public:
    sync_signals();

    binary_semaphore *ready_request();

    ~sync_signals();
};