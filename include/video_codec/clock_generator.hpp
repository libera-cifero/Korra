#pragma once
#include "sync_signals.hpp"
#include <cstdint>
#include <semaphore>
#include <thread>

using namespace std;
class clock_generator {
private:
    sync_signals *_signals;
    binary_semaphore *_breaker;
    uint32_t _timeout;
    bool _is_running;
    thread *_clock_thread;
public:
    clock_generator(uint32_t timeout);

    int timeout();

    sync_signals *signals();

    void launch();

    ~clock_generator();
};