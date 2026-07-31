#include "data_boxer/data/begin_label_data.hpp"
#include "data_boxer/data/korra_data.hpp"
#include <cstdint>
#include <cstring>


begin_label_data::begin_label_data() { }

begin_label_data::begin_label_data(uint16_t package_type, int id, uint16_t data_size): identifiable(id, data_size) {
    _payload_type = package_type;
}

begin_label_data::begin_label_data(uint16_t package_type, uint16_t data_size) : identifiable(data_size) {
    _payload_type = package_type;
}

uint16_t begin_label_data::payload_type(){ return _payload_type; }

int begin_label_data::size() { 
    return 8;
}

int begin_label_data::size(char *bytes) {
    int data_size;
    memcpy(&data_size, bytes + 8, 2);
    return data_size;
}

uint16_t begin_label_data::type() { return 1; }
char *begin_label_data::to_bytes(char *buffer) {
    buffer = korra_data::to_bytes(buffer);

    memcpy(buffer, &_payload_type, 2);
    memcpy(buffer + 2, &$id, 4);
    memcpy(buffer + 6, &$data_size, 2);

    return buffer + size();
}

char *begin_label_data::from_bytes(char *bytes) {
    bytes = korra_data::from_bytes(bytes);

    memcpy(&_payload_type, bytes, 2);
    memcpy(&$id, bytes + 2, 4);
    memcpy(&$data_size, bytes + 6, 2);

    return bytes + size();
}