#pragma once
#include "cipher.hpp"
#include "config/data/cipher_config.hpp"

class none_cipher : public cipher {
public:
    none_cipher(cipher_config &config);
    int header_size() override;
    char *encrypt(char *data) override;
    char *decrypt(char *encrypted) override;
};