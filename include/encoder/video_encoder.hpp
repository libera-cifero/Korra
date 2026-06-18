#include "compressor/compressor.hpp"
#include "obfuscator/obfuscator.hpp"
#include "recoverer/recoverer.hpp"
#include "provider/provider.hpp"

typedef struct video_encoder_config {
    provider *_provider;
    recoverer *_recoverer;
    compressor *_compressor;
    obfuscator *_obfuscator;
} video_encoder_config;

class video_encoder {
private:
    video_encoder_config _config;
public:
    video_encoder(video_encoder_config config);
    char **encode_package(char *ip_package);
    char *decode_package(char **frames);
    ~video_encoder();
};