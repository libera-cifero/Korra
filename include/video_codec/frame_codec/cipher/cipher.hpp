#include "config/data/cipher_config.hpp"
#include <concepts>
#pragma once

class cipher {
private:
    cipher_config *_config;
protected:
    template<std::derived_from<cipher_config> T>
    T *__config();
public:
    cipher(cipher_config *config);
    int payload_size();
    int encrypted_size();
    virtual int header_size() = 0;
    virtual char *encrypt(char *data) = 0;
    virtual char *decrypt(char *encrypted) = 0;
    virtual ~cipher();
};