#pragma once
#include "cipher/cipher.hpp"
#include "provider/provider.hpp"

class frame_codec {
private:
    provider *_provider;
    cipher *_cipher;
    bool _is_void_payload(char *frame);
public:
    frame_codec(provider *provider, cipher *cipher);

    provider *get_provider();
    cipher *get_cipher();

    int payload_size();
    int frame_size();

    char *encode(char *data);
    char *decode(char *frame);
    ~frame_codec();
};