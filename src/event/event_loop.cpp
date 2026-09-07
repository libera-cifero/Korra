#include "event/event_loop.hpp"
#include <chrono>
#include <cstddef>
#include <functional>
#include <thread>
#include <unistd.h>
#include <vector>

event_loop::event_loop(size_t delay_microseconds){
    _delay = delay_microseconds;
}

void event_loop::observe(event *e){
    _listening_events.push_back(e);
}

void event_loop::subscribe(string event_name, function<void(event_args *)> action){
    if(!_loop_executor.contains(event_name)){
        _loop_executor[event_name] = new vector<function<void(event_args *)>>;
    }
    _loop_executor[event_name]->push_back(action);
}

void event_loop::run(){
    _is_running = true;
    while(_is_running){
        for(auto e : _listening_events){
            auto result = e->check();
            if(result != nullptr){
                auto funcs = *_loop_executor[e->name()];
                for(auto action : funcs) action(result);
            }
            delete result;
        }
        std::this_thread::sleep_for(std::chrono::microseconds(_delay));
    }
}

event_loop::~event_loop(){
    _is_running = false;
    for(auto kvp : _loop_executor){
        delete kvp.second;
    }
}