#include "event/event_loop.hpp"
#include "lib/log.hpp"
#include <chrono>
#include <cstddef>
#include <functional>
#include <spdlog/spdlog.h>
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
    if(!_event_handlers.contains(event_name)){
        _event_handlers[event_name] = new vector<function<void(event_args *)>>;
    }
    _event_handlers[event_name]->push_back(action);
}

void event_loop::run(){
    _is_running = true;
    while(_is_running){
        for(auto e : _listening_events){
            auto result = e->check();
            if(result != nullptr){
                auto handlers = *_event_handlers[e->name()];
                for(auto handler : handlers) handler(result);
            }
            delete result;
        }
        std::this_thread::sleep_for(std::chrono::microseconds(_delay));
    }
}

void event_loop::stop(){
    _is_running = false;
}

event_loop::~event_loop(){
    auto prefix = get_method_prefix("event_loop.~event_loop");
    spdlog::debug("{} destructing...", prefix);
    _is_running = false;
    for(auto kvp : _event_handlers){
        delete kvp.second;
    }
    spdlog::debug("{} event_loop was destucted!", prefix);
}