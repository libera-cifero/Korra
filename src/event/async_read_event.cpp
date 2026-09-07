#include "event/async_read_event.hpp"

async_read_event::async_read_event(string event_name, function<char*()> reader){
    _reader = reader;
    _name = event_name;
}

event_args *async_read_event::check(){
    char *data = _reader();
    if(data != nullptr){
        return new async_read_event_args { .data = data };
    }
    return nullptr;
}