#include "oscillator/clock_generator.hpp"
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

void clock_generator::launch(){
    _clock_thread = new thread([&](){
        int root_timeout = _timeout - _ready_gap_timeout;
        _is_running = true;
        while(_is_running) {
            this_thread::sleep_for(chrono::milliseconds(root_timeout));
            clock_t t0 = clock();
            _ready_request->release();
            _ready_response->acquire();
            int t = (int)(1000.0 * (clock() - t) / CLOCKS_PER_SEC);
            if(t < _ready_gap_timeout)
                this_thread::sleep_for(chrono::milliseconds(_ready_gap_timeout - t));
            
            _frame_request->release();
        }
    });
    _clock_thread -> detach();
}

clock_generator::~clock_generator(){
    _is_running = false;
    _clock_thread->join();
    delete _ready_request;
    delete _ready_response;
    delete _frame_request;
    delete _clock_thread;
}