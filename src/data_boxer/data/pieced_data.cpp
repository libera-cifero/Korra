#include "data_boxer/data/pieced_data.hpp"
#include "data_boxer/data/korra_data.hpp"
#include <cstdint>
#include <cstring>

pieced_data::pieced_data() { }

pieced_data::pieced_data(int id, uint16_t piece_size, char *piece) : identifiable(id, piece_size){
    _piece = piece;
    _index = 0;
}

char *pieced_data::piece() { return _piece; }
uint16_t pieced_data::index() { return _index; }

int pieced_data::size() { return 8 + __data_size; }
int pieced_data::size(char *bytes) {
    int data_size;
    memcpy(&data_size, bytes + 8, 2);
    return data_size;
}

uint16_t pieced_data::type() { return 2; }

char *pieced_data::to_payload_bytes(char *buffer) {
    memcpy(buffer, &__id, 4);
    memcpy(buffer + 4, &__data_size, 2);
    memcpy(buffer + 6, &_index, 2);
    memcpy(buffer + 8, _piece, __data_size);
    return buffer + size();
}

char *pieced_data::from_bytes(char *bytes) {
    bytes = korra_data::from_bytes(bytes);

    memcpy(&__id, bytes, 4);
    memcpy(&__data_size,bytes + 4, 2);
    memcpy(&_index,bytes + 6, 2);
    _piece = new char[__data_size];
    memcpy(_piece, bytes + 8, __data_size);

    return bytes + size();
}

pieced_data::~pieced_data() {
    //_piece is not deleting, because _piece is processing by out agent
}