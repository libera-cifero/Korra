#include "event/timer_event.hpp"
#include "lib/time.hpp"
#include <ctime>

timer_event::timer_event(int timeout_millis){
    _timout_millis = timeout_millis;
    _is_first_check = true;
    _previous_time = { .tv_sec = 0, .tv_nsec = 0 };
}

event_args *timer_event::check(){
    if(_is_first_check){
        clock_gettime(CLOCK_MONOTONIC, &_previous_time);
        _is_first_check = false;
        return nullptr;
    }

    timespec current;
    clock_gettime(CLOCK_MONOTONIC, &current);
    if(get_delta_millis(_previous_time, current) >= _timout_millis){
        _previous_time = current;
        return new timer_event_args;
    }
    return nullptr;
}