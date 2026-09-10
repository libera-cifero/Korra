#pragma once
#include "lib/string_literal.hpp"
#include <string>
using namespace std;
struct event_args { 
    virtual ~event_args() = default;
};

class event {
public:
    virtual string name() = 0;
    virtual event_args *check() = 0;
    virtual ~event() = default;
};

template<string_literal event_name>
class static_named_event: public event {
public:
    static constexpr string NAME = event_name.value;
    string name() override {
        return NAME;
    }
};