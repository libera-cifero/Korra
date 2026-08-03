#include "video_codec/clock_generator.hpp"
#include "video_codec/sync_signals.hpp"
#include <chrono>
#include <ctime>
#include <semaphore>
#include <stdexcept>
#include <thread>
#include <boost/asio.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/steady_timer.hpp>

clock_generator::clock_generator(uint32_t timeout, uint32_t ready_gap_timeout){
    if(ready_gap_timeout >= timeout) throw runtime_error("Ready timeout greator or equal of timeout!");

    _ready_gap_timeout = ready_gap_timeout;
    _timeout = timeout;

    _signals = new sync_signals;
    _breaker = new binary_semaphore(0);
}

int clock_generator::timeout() {
    return _timeout;
}

int clock_generator::ready_gap_timeout() {
    return _ready_gap_timeout;
}

sync_signals *clock_generator::signals(){
    return _signals;
}

static double get_delta_millis(timespec t0, timespec t1){
    return ((t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec) / 1e9) * 1000.0;
}

void clock_generator::launch(){
    _clock_thread = new thread([&](){
        int root_timeout = _timeout - _ready_gap_timeout;
        _is_running = true;
        timespec t0, t1;
        auto ready_request = _signals -> ready_request(), ready_response = _signals -> ready_response(), frame_request = _signals -> frame_request();
        while(_is_running) {
            this_thread::sleep_for(chrono::milliseconds(root_timeout));
            clock_gettime(CLOCK_MONOTONIC, &t0);
            ready_request -> release();
            ready_response -> acquire();
            clock_gettime(CLOCK_MONOTONIC, &t1);
            int t = (int)get_delta_millis(t0, t1);
            if(t < _ready_gap_timeout)
                this_thread::sleep_for(chrono::milliseconds(_ready_gap_timeout - t)); 
            frame_request -> release();
        }
        _breaker->release();
    });
    _clock_thread -> detach();
}

clock_generator::~clock_generator(){
    _is_running = false;
    _signals-> ready_response() -> release();
    _breaker->acquire();
    delete _signals;
    delete _breaker;
    delete _clock_thread;
}