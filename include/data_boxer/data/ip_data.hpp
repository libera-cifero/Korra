#pragma once
#include "piecable_data.hpp"
#include <cstdint>


/*
    Wrapper for IP package

    BYTE's STRUCTURE
    +-----------+--------------+
    | TYPE = 0  | PAYLOAD DATA |
    | 2 bytes   |   N BYTES    |
    +-----------+--------------+
                |  IP PACKAGE  |
                |   N bytes    |
                +--------------+
*/
class ip_data : public piecable_data {
public:
    ip_data();
    ip_data(uint16_t package_size);
    ip_data(char *ip_package);

    char *ip_package();

    int size() override;
    int size(char *bytes) override;

    uint16_t type() override;
    char *to_bytes(char *buffer) override;
    char *from_bytes(char *bytes) override;

    ~ip_data();
};