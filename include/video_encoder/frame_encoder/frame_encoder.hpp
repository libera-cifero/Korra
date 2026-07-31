#pragma once
#include "obfuscator/obfuscator.hpp"
#include "provider/provider.hpp"

class frame_encoder {
private:
    provider *_provider;
    obfuscator *_obfuscator;
public:
    frame_encoder(provider *provider, obfuscator *obfuscator);

    int payload_size();
    int frame_size();

    char *encode(char *data);
    char *decode(char *frame);
    ~frame_encoder();
};