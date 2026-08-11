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

clock_generator::clock_generator(uint32_t timeout){
    _timeout = timeout;

    _signals = new sync_signals;
    _breaker = new binary_semaphore(0);
}

int clock_generator::timeout() {
    return _timeout;
}
sync_signals *clock_generator::signals(){
    return _signals;
}

static double get_delta_millis(timespec t0, timespec t1){
    return ((t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec) / 1e9) * 1000.0;
}

void clock_generator::launch(){
    _clock_thread = new thread([&](){
        
        _is_running = true;
        timespec t0, t1;
        auto ready_request = _signals -> ready_request();
        while(_is_running) {
            this_thread::sleep_for(chrono::milliseconds(_timeout));
            ready_request -> release();
        }
        _breaker->release();
    });
    _clock_thread -> detach();
}

clock_generator::~clock_generator(){
    _is_running = false;
    _breaker->acquire();
    delete _signals;
    delete _breaker;
    delete _clock_thread;
}