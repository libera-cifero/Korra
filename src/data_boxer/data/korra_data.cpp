#include "data_boxer/data/korra_data.hpp"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <stdexcept>

korra_data::korra_data() { }

int korra_data::total_size(){
    return 2 + size();
}

char* korra_data::to_bytes(char *buffer){
    uint16_t t = type();
    memcpy(buffer, &t, 2);
    return buffer + 2;
}

uint16_t korra_data::type(char *bytes){
    uint16_t t = 0;
    memcpy(&t, bytes, 2);
    return t;
}

char *korra_data::from_bytes(char *bytes){
    uint16_t t = type(bytes);
    uint16_t data_type = type();
    if(t != data_type){
        char buffer[256];
        sprintf(buffer,"Invalid data type! To parse bytes, it must be %d, but got %d!", data_type, t);
        throw std::runtime_error(buffer);
    }

    return bytes + 2;
}

korra_data::~korra_data() { }