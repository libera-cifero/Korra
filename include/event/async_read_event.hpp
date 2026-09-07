#pragma once
#include "event.hpp"
#include <functional>

struct async_read_event_args : public event_args {
    char *data;
};

class async_read_event : public event {
private:
    string _name;
    function<char*()> _reader;
public:
    async_read_event(string event_name, function<char*()> reader);
    string name() override;
    event_args *check() override;
};