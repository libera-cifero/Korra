#include "video_encoder/clock_generator.hpp"
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

    _ready_request = new binary_semaphore(0);
    _ready_response = new binary_semaphore(0);
    _frame_request = new binary_semaphore(0);
    _breaker = new binary_semaphore(0);
}

int clock_generator::timeout() {
    return _timeout;
}

int clock_generator::ready_gap_timeout() {
    return _ready_gap_timeout;
}

binary_semaphore *clock_generator::ready_request() {
    return _ready_request;
}

binary_semaphore *clock_generator::ready_response() {
    return _ready_response;
}

binary_semaphore *clock_generator::frame_request() {
    return _frame_request;
}

static double get_delta_millis(timespec t0, timespec t1){
    return ((t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec) / 1e9) * 1000.0;
}

void clock_generator::launch(){
    _clock_thread = new thread([&](){
        int root_timeout = _timeout - _ready_gap_timeout;
        _is_running = true;
        timespec t0, t1;
        while(_is_running) {
            this_thread::sleep_for(chrono::milliseconds(root_timeout));
            clock_gettime(CLOCK_MONOTONIC, &t0);
            _ready_request->release();
            _ready_response->acquire();
            clock_gettime(CLOCK_MONOTONIC, &t1);
            int t = (int)get_delta_millis(t0, t1);
            if(t < _ready_gap_timeout)
                this_thread::sleep_for(chrono::milliseconds(_ready_gap_timeout - t)); 
            _frame_request->release();
        }
        _breaker->release();
    });
    _clock_thread -> detach();
}

clock_generator::~clock_generator(){
    _is_running = false;
    _ready_response->release();
    _breaker->acquire();
    delete _ready_request;
    delete _ready_response;
    delete _frame_request;
    delete _clock_thread;
    delete _breaker;
}