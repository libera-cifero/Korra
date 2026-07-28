#pragma once
#include "korra_data.hpp"
#include "base/piecable.hpp"
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
class ip_data : public korra_data, public piceable {
private:
    char *_ip_package;
    uint16_t _package_size;
    char *_current_piece;
public:
    ip_data();
    ip_data(uint16_t package_size);
    ip_data(char *ip_package);

    char *ip_package();

    bool is_splittable() override;

    int size() override;
    int size(char *bytes) override;

    uint16_t type() override;
    char *to_bytes() override;
    void from_bytes(char *bytes) override;

    void append_piece(char *piece, int piece_size) override;

    ~ip_data();
};