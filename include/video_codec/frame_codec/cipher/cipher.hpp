#include "config/data/cipher_config.hpp"
#include <concepts>
#pragma once

class cipher {
private:
    cipher_config *_config;
protected:
public:
    cipher(cipher_config *config);
    int payload_size();
    int encrypted_size();
    template<std::derived_from<cipher_config> T>
    T *config(){ return static_cast<T*>(_config); }
    virtual int header_size() = 0;
    virtual char *encrypt(char *data) = 0;
    virtual char *decrypt(char *encrypted) = 0;
    virtual ~cipher();
};