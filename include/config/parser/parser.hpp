#pragma once
#include "lib/json.hpp"
#include <string>
#include <vector>
using json = nlohmann::json;
using namespace std;

template<typename T>
class json_parser {
public:
    virtual T parse(json) = 0;
    virtual json serialize(T) = 0;
    virtual ~json_parser() {}
};

template<typename T>
class specific_parser : public json_parser<T> {
public:
    virtual string type() = 0;
    virtual bool can_serialize(T) = 0;
};

template<typename T>
class abstract_parser : public json_parser<T> {
public:
//it sets up in parser_provider
    vector<specific_parser<T>*> specific_parsers;
    virtual ~abstract_parser() {
        for(auto p : specific_parsers) delete p;
    }
};

template<typename T>
class contexted_in {
public:
    T context_in;
};

template<typename T>
class contexted_out {
public:
    T context_out;
};