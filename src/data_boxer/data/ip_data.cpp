#include "data_boxer/data/ip_data.hpp"
#include <cstdint>
#include <cstring>
#include <stdexcept>

ip_data::ip_data() : piecable_data() { 
    _ip_package = nullptr;
}

ip_data::ip_data(uint16_t package_size){
    _package_size = package_size;
    _ip_package = new char[_package_size];
    _current_piece = _ip_package;
}

ip_data::ip_data(char *ip_package){
    _ip_package = ip_package;
}

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

char *ip_data::to_bytes(char *buffer) {
    if(_ip_package == nullptr) throw std::runtime_error("IP package is not defined!");
    char *bytes = korra_data::to_bytes(buffer);
    memcpy(bytes, _ip_package, size());
    return bytes + size();
}

char *ip_data::from_bytes(char *bytes) {
    bytes = korra_data::from_bytes(bytes);
    uint16_t length = size(bytes);

    if(_ip_package != nullptr) delete [] _ip_package;
    _ip_package = new char[length];

    memcpy(_ip_package, bytes, length);
    return bytes + size();
}

void ip_data::read_piece(char* piece_buffer, int piece_size, int &byte_index){

}

void ip_data::append_piece(char *piece, int piece_size) {
    memcpy(_current_piece, piece, piece_size);
    _current_piece += piece_size;
}

ip_data::~ip_data(){
    if(_ip_package != nullptr) delete [] _ip_package;
}