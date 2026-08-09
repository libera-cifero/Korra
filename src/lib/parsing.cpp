#include "lib/parsing.hpp"
#include <cstdlib>
#include <stdexcept>
#include <string>

using namespace std;
int parse_int(nlohmann::basic_json<> token) {
    if(token.is_string()) return atoi(token.get<string>().c_str());
    else if(token.is_number()) return token.get<int>();
    else throw runtime_error("Can't parse int!");
}

uint32_t parse_uint32_t(nlohmann::basic_json<> token) {
    if(token.is_string()) return (uint32_t)atol(token.get<string>().c_str());
    else if(token.is_number_unsigned()) return token.get<int>();
    else throw runtime_error("Can't parse uint32_t!");
}

bool parse_bool(nlohmann::basic_json<> token) {
    if(token.is_string()) 
    {
        string val = token.get<string>();
        if(val == "true") return true;
        else if(val == "false") return false;
        else throw runtime_error("Can't parse bool!");
    }
    else if(token.is_boolean()) return token.get<bool>();
    else throw runtime_error("Can't parse bool!");
}