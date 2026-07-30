#pragma once
#include "korra_data.hpp"
#include "base/identifiable.hpp"
#include <cstdint>

/*
    Wrapper, that contains piece of large data.


    BYTE's STRUCTURE
    +-----------+---------------------------------------------+
    | TYPE = 2  |              PAYLOAD DATA                   |
    | 2 bytes   |              8 + N bytes                    |
    +-----------+---------+---------+------------+------------+
                |   ID    |  INDEX  | DATA SIZE  | PIECE DATA |
                | 4 bytes | 2 bytes |  2 bytes   |  N bytes   |
                +---------+---------+------------+------------+
*/

class pieced_data : public korra_data, public identifiable {
private:
    char *_piece;
    uint16_t _index;
public:
    pieced_data();
    pieced_data(int id, uint16_t piece_size, char *piece);

    //min size of payload
    static const int MIN_SIZE = 8;

    static const int MIN_TOTAL_SIZE = 10;

    char *piece();
    uint16_t index();

    int size() override;
    int size(char *bytes) override;

    uint16_t type() override;
    char *to_bytes() override;
    void from_bytes(char *bytes) override;

    ~pieced_data();
};