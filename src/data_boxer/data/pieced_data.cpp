#include "data_boxer/data/pieced_data.hpp"
#include "data_boxer/data/korra_data.hpp"
#include <cstdint>
#include <cstring>

pieced_data::pieced_data() { }

char *pieced_data::piece() { return _piece; }
uint16_t pieced_data::index() { return _index; }

bool pieced_data::is_splittable() { return false; }

int pieced_data::size() { return 8 + data_size(); }
int pieced_data::size(char *bytes) {
    int data_size;
    memcpy(&data_size, bytes + 8, 2);
    return data_size;
}

uint16_t pieced_data::type() { return 2; }

char *pieced_data::to_bytes() {
    char *bytes = korra_data::to_bytes();
    memcpy(bytes + 2, &$id, 4);
    memcpy(bytes + 6, &$data_size, 2);
    memcpy(bytes + 8, &_index, 2);
    memcpy(bytes + 10, _piece, $data_size);
    return bytes;
}

void pieced_data::from_bytes(char *bytes) {
    korra_data::from_bytes(bytes);

    memcpy(&$id, bytes + 2, 4);
    memcpy(&$data_size,bytes + 6, 2);
    memcpy(&_index,bytes + 8, 2);
    if(_piece != nullptr) delete [] _piece;
    _piece = new char[$data_size];
    memcpy(_piece, bytes + 10, $data_size);
}

pieced_data::~pieced_data() {
    if(_piece != nullptr) delete [] _piece;
}