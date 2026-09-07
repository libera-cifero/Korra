#include "event/timer_event.hpp"
#include "lib/time.hpp"
#include <ctime>

timer_event::timer_event(int timeout_millis){
    _timout_millis = timeout_millis;
    clock_gettime(CLOCK_MONOTONIC, &_previous_time);
}

event_args *timer_event::check(){
    timespec current;
    clock_gettime(CLOCK_MONOTONIC, &current);
    if(get_delta_millis(_previous_time, current) >= _timout_millis){
        _previous_time = current;
        return new timer_event_args;
    }
    return nullptr;
}