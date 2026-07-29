#pragma once
#include <cstdint>


/*
    Abstract wrapper over raw bytes

    BYTE's STRUCTURE
    +-----------+----------------+
    |   TYPE    |  PAYLOAD DATA  |
    |  2 bytes  |    N bytes     |
    +-----------+----------------+
*/
class korra_data {
public:
    korra_data();

    virtual uint16_t type() = 0;
    uint16_t type(char *bytes);

    int total_size();

    //Return size of payload data in bytes
    virtual int size() = 0;
    //Return size of payload data of raw_bytes
    virtual int size(char *raw_bytes) = 0;
    
    virtual char *to_bytes();
    virtual void from_bytes(char *bytes);

    virtual ~korra_data();
};