#include "data_boxer/data/begin_label_data.hpp"
#include "data_boxer/data/korra_data.hpp"
#include <cstdint>
#include <cstring>


begin_label_data::begin_label_data() { }

begin_label_data::begin_label_data(uint16_t package_type, int id, uint16_t total_size): identifiable(id, total_size) {
    _payload_type = package_type;
}

begin_label_data::begin_label_data(uint16_t package_type, uint16_t total_size) : identifiable(total_size) {
    _payload_type = package_type;
}

uint16_t begin_label_data::payload_type(){ return _payload_type; }

bool begin_label_data::is_splittable(){ return false; }

int begin_label_data::size() { 
    return 8;
}
int begin_label_data::size(char *bytes) {
    int data_size;
    memcpy(&data_size, bytes + 8, 2);
    return data_size;
}

uint16_t begin_label_data::type() { return 1; }
char *begin_label_data::to_bytes() {
    char *bytes = korra_data::to_bytes();

    memcpy(bytes + 2, &_payload_type, 2);
    memcpy(bytes + 4, &$id, 4);
    memcpy(bytes + 8, &$data_size, 2);

    return bytes;
}

void begin_label_data::from_bytes(char *bytes) {
    korra_data::from_bytes(bytes);

    memcpy(&_payload_type, bytes + 2, 2);
    memcpy(&$id, bytes + 4, 4);
    memcpy(&$data_size, bytes + 8, 2);
}