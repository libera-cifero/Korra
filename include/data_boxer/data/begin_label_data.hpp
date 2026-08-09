#pragma once
#include "korra_data.hpp"
#include "base/identifiable.hpp"
#include <cstdint>

/*
    Label signing a beginning of piced_data array 


    BYTE's STRUCTURE
    +-----------+--------------------------------------+
    | TYPE = 1  |            PAYLOAD DATA              |
    | 2 bytes   |              8 bytes                 |
    +-----------+----------------+---------+-----------+
                |  PAYLOAD TYPE  |   ID    | DATA SIZE |
                |    2 bytes     | 4 bytes |  2 bytes  |
                +----------------+---------+-----------+
*/

class begin_label_data : public korra_data, public identifiable {
private:
    uint16_t _payload_type;
public:
    begin_label_data();
    begin_label_data(uint16_t package_type, int id, uint16_t data_size);
    begin_label_data(uint16_t package_type, uint16_t data_size);

    uint16_t payload_type();

    int size() override;
    int size(char *bytes) override;

    uint16_t type() override;
    char *to_bytes(char *buffer) override;
    char *from_bytes(char *bytes) override;
};