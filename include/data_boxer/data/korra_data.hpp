#pragma once
#include <cstdint>

class korra_data {
public:
    korra_data();

    virtual bool is_splittable() = 0;

    virtual uint16_t type() = 0;
    uint16_t type(char *bytes);

    //Return size of payload data in bytes
    virtual int size() = 0;
    //Return size of payload data of raw_bytes
    virtual int size(char *raw_bytes) = 0;
    
    virtual char *to_bytes();
    virtual void from_bytes(char *bytes);

    virtual ~korra_data();
};