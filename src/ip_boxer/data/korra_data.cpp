#include "ip_boxer/data/korra_data.hpp"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <stdexcept>

char* korra_data::to_bytes(){
    int total_size = 2 + size();
    char *bytes = new char[total_size];
    memset(bytes, 0, total_size);
    uint16_t t = type();
    memcpy(bytes, &t, 2);
    return bytes;
}

uint16_t korra_data::type(char *bytes){
    uint16_t t = 0;
    memcpy(&t, bytes, 2);
    return t;
}

void korra_data::from_bytes(char *bytes){
    uint16_t t = type(bytes);
    uint16_t data_type = type();
    if(t != data_type){
        char buffer[256];
        sprintf(buffer,"Invalid data type! To parse bytes, it must be %d, but got %d!", data_type, t);
        throw std::runtime_error(buffer);
    }
}

korra_data::~korra_data() { }