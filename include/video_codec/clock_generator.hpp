#pragma once
#include "sync_signals.hpp"
#include <cstdint>
#include <thread>

using namespace std;
class clock_generator {
private:
    sync_signals *_signals;
    uint32_t _timeout;
    thread _loop_thread;
    bool _is_running;
public:
    clock_generator(uint32_t timeout);

    int timeout();

    sync_signals *signals();

    void launch();

    ~clock_generator();
};