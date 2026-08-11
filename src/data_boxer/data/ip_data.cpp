#include "data_boxer/data/ip_data.hpp"
#include "data_boxer/data/piecable_data.hpp"
#include <cstdint>
#include <cstring>
#include <stdexcept>

ip_data::ip_data() : piecable_data() { }

ip_data::ip_data(uint16_t package_size){
    init_buffer(package_size);
    memcpy(__buffer + 2, &__buffer_size, 2);
}

ip_data::ip_data(char *ip_package){
    int buffer_size = size(ip_package);
    init_buffer(buffer_size);
    memcpy(__buffer, ip_package, size());
}

uint16_t ip_data::type() { return 0; }

int ip_data::size(){ return piecable_data::size(); }

int ip_data::size(char *raw_bytes){
    uint16_t size = 0;
    memcpy(&size, raw_bytes + 2, 2);
    return size;
}

char *ip_data::ip_package() { 
    return __buffer;
}

char *ip_data::to_payload_bytes(char *buffer) {
    if(__buffer == nullptr) throw std::runtime_error("IP package is not defined!");
    memcpy(buffer, __buffer, size());
    return buffer + size();
}

char *ip_data::from_bytes(char *bytes) {
    bytes = korra_data::from_bytes(bytes);
    uint16_t length = size(bytes);

    if(__buffer != nullptr) delete [] __buffer;
    
    init_buffer(length);

    memcpy(__buffer, bytes, length);
    return bytes + size();
}

ip_data::~ip_data() { 
    //_ip_package is not deleting, because _ip_package is processing by out agent
}