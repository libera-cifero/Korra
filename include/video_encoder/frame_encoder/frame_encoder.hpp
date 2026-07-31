#pragma once
#include "cipher/cipher.hpp"
#include "provider/provider.hpp"

class frame_encoder {
private:
    provider *_provider;
    cipher *_cipher;
public:
    frame_encoder(provider *provider, cipher *cipher);

    int payload_size();
    int frame_size();

    char *encode(char *data);
    char *decode(char *frame);
    ~frame_encoder();
};