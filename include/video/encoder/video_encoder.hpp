#pragma once
#include "obfuscator/obfuscator.hpp"
#include "corrector/corrector.hpp"
#include "provider/provider.hpp"
#include <queue>

typedef struct video_encoder_config {
    provider *provider;
    corrector *corrector;
    obfuscator *obfuscator;
} video_encoder_config;

class video_encoder {
private:
    video_encoder_config *_config;
    queue<char> _payload_in_bytes;
    queue<char> _payload_out_bytes;
    char *_frame;
public:
    video_encoder(video_encoder_config *config);
    video_encoder_config *config();
    char *eject_frame();
    char *encode(char *data);
    char *decode(char *frame);
    ~video_encoder();
};