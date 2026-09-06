#include "lib/log.hpp"
#include <cstdint>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <format>
#include <thread>

string get_method_prefix(string method_name){
    return format("[THREAD {}] {}:", this_thread::get_id(), method_name);
}

bool is_debug_level(){
    return spdlog::get_level() == spdlog::level::debug;
}

string get_byte_str(void *bytes){
    if(bytes == nullptr || bytes == NULL)
        return "NULL";
    
    uint8_t *data = reinterpret_cast<uint8_t*>(bytes);
    string str = "";
    for(int i = 0; i < BYTE_STR_DISPLAY_COUNT; i++){
        if(i>0) str+=" ";
        str+=to_string(data[i]);
    }

    str+="...";
    return str;
}