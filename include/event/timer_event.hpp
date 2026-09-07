#pragma once
#include "event.hpp"
#include <ctime>

struct timer_event_args : event_args { };

class timer_event : public static_named_event<"timer_event"> {
private:
    timespec _previous_time;
    int _timout_millis;
public:
    timer_event(int timeout_millis);
    event_args *check() override;
};