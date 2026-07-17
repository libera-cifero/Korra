#pragma once
#include "json.hpp"
#include <string>
#include <vector>
using json = nlohmann::json;
using namespace std;

template<typename T>
class json_parser {
public:
    virtual T parse(json) = 0;
    virtual json serialize(T) = 0;
    virtual ~json_parser<T>() {}
};

template<typename T>
class specific_parser : public json_parser<T> {
public:
    virtual string type() = 0;
};

template<typename T>
class abstract_parser : public json_parser<T> {
public:
//it sets up in parser_provider
    vector<specific_parser<T>*> specific_parsers;
};