#include "data_boxer/data/ip_data.hpp"
#include <cstdint>
#include <cstring>
#include <stdexcept>

ip_data::ip_data() : korra_data() { 
    _ip_package = nullptr;
}

ip_data::ip_data(char *ip_package){
    _ip_package = ip_package;
}

bool ip_data::is_splittable() { return true; }

uint16_t ip_data::type() { return 0; }

int ip_data::size() {
    uint16_t size = 0;
    memcpy(&size, _ip_package + 2, 2);
    return size;
}

int ip_data::size(char *raw_bytes){
    uint16_t size = 0;
    memcpy(&size, raw_bytes + 4, 2);
    return size;
}

char *ip_data::ip_package() { return _ip_package; }

char *ip_data::to_bytes() {
    if(_ip_package == nullptr) throw std::runtime_error("IP package is not defined!");
    char *bytes = korra_data::to_bytes();
    memcpy(bytes + 2, _ip_package, size());
    return bytes;
}

void ip_data::from_bytes(char *bytes) {
    korra_data::from_bytes(bytes);
    uint16_t length = size(bytes);

    if(_ip_package != nullptr) delete [] _ip_package;
    _ip_package = new char[length];

    memcpy(_ip_package, bytes + 2, length);
}

ip_data::~ip_data(){
    if(_ip_package != nullptr) delete [] _ip_package;
}