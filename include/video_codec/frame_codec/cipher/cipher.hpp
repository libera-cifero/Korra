#include "config/data/cipher_config.hpp"
#pragma once

class cipher {
protected:
    cipher_config __config;
public:
    cipher(cipher_config &config);
    int payload_size();
    int encrypted_size();
    virtual int header_size() = 0;
    virtual char *encrypt(char *data) = 0;
    virtual char *decrypt(char *encrypted) = 0;
    virtual ~cipher();
};