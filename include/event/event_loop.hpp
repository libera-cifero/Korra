#pragma once
#include "event.hpp"
#include <cstddef>
#include <functional>
#include <map>
#include <string>
#include <vector>

class event_loop {
private:
    vector<event *> _listening_events;
    map<string, vector<function<void(event_args*)>>*> _loop_executor;
    bool _is_running = false;
    size_t _delay;
public:
    event_loop(size_t tick_delay_microseconds);
    void observe(event *e);
    void subscribe(string event_name, function<void(event_args *)> action);
    void run();
    ~event_loop();
};