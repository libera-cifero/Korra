#pragma once
#include "obfuscator/obfuscator.hpp"
#include "provider/provider.hpp"

class video_encoder {
private:
    provider *_provider;
    obfuscator *_obfuscator;
public:
    video_encoder(provider *provider, obfuscator *obfuscator);

    int payload_size();
    int frame_size();

    char *encode(char *data);
    char *decode(char *frame);
    ~video_encoder();
};