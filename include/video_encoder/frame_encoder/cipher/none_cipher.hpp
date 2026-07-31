#pragma once
#include "cipher.hpp"

class none_cipher : public cipher {
public:
    none_cipher(int payload_size);
    char *encrypt(char *data) override;
    char *decrypt(char *encrypted) override;
};