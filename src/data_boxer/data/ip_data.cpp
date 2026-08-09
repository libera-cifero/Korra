#include "data_boxer/data/ip_data.hpp"
#include <cstdint>
#include <cstring>
#include <stdexcept>

ip_data::ip_data() : piecable_data() { }

ip_data::ip_data(uint16_t package_size){
    init_buffer(package_size);
}

ip_data::ip_data(char *ip_package){
    int s = size(ip_package);
    init_buffer(s);
    memcpy($buffer, ip_package, size());
}

uint16_t ip_data::type() { return 0; }

int ip_data::size(char *raw_bytes){
    uint16_t size = 0;
    memcpy(&size, raw_bytes + 4, 2);
    return size;
}

char *ip_data::ip_package() { return $buffer; }

char *ip_data::to_bytes(char *buffer) {
    if($buffer == nullptr) throw std::runtime_error("IP package is not defined!");
    char *bytes = korra_data::to_bytes(buffer);
    memcpy(bytes, $buffer, size());
    return bytes + size();
}

char *ip_data::from_bytes(char *bytes) {
    bytes = korra_data::from_bytes(bytes);
    uint16_t length = size(bytes);

    if($buffer != nullptr) {
        delete [] $buffer;
        init_buffer(length);
    }

    memcpy($buffer, bytes, length);
    return bytes + size();
}

ip_data::~ip_data() { 
    //_ip_package is not deleting, because _ip_package is processing by out agent
}