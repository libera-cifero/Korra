#include "video/timer.hpp"
#include <boost/system/system_error.hpp>
#include <chrono>
#include <exception>
#include <functional>
#include <mutex>
#include <thread>
#include <boost/asio/error.hpp>
#include <iostream>

cancellable_clock timer::_make_clock(){
    auto clock = new boost::asio::basic_waitable_timer<std::chrono::steady_clock>(_io);
    clock->expires_after(std::chrono::milliseconds(_timeout_millis));
    return clock;
}

timer::timer(int millis){
    _timeout_millis = millis;
    _is_running = false;
    _is_destroying = false;
}

void timer::launch(){
    if(_is_running) return;
    _thread = jthread([&]() 
    {
        _cancellable_clock = _make_clock();

        _is_running = true;
        while(true) {
            bool is_canceled = false;
            try{
                _cancellable_clock->wait();
            }
            catch(boost::system::system_error &e){
                if(e.code() == boost::asio::error::operation_aborted) is_canceled = true;
                else throw e;
            }

            if(_is_destroying) break;

            lock_guard<mutex> lock(_clock_mutex);
            delete _cancellable_clock;

            try{
                if(!is_canceled) for(auto event : _events) event();
            }
            catch(exception &e){
                cerr << e.what() << endl;
            }
            catch (...){}
            _cancellable_clock = _make_clock();
        }
    });
}

void timer::reset(){
    if(!_is_running) return;
    lock_guard<mutex> lock(_clock_mutex);
    _cancellable_clock->cancel();
}

int timer::timeout_millis(){
    return _timeout_millis;
}

void timer::on_timed_out(function<void()> &event){
    _events.push_back(event);
}

timer::~timer(){
    if(_is_running){
        _is_destroying = true;
        reset();       
        _thread.join();
        delete _cancellable_clock;
    }
}