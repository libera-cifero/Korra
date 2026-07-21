#pragma once
#include "compressor/compressor.hpp"
#include "obfuscator/obfuscator.hpp"
#include "corrector/corrector.hpp"
#include "provider/provider.hpp"
#include "video_frame.hpp"
#include <cstddef>
#include <vector>

typedef struct video_encoder_config {
    provider *provider;
    corrector *corrector;
    compressor *compressor;
    obfuscator *obfuscator;
} video_encoder_config;

class video_encoder {
private:
    video_encoder_config *_config;
public:
    video_encoder(video_encoder_config *config);
    std::vector<video_frame>& encode_package(void *ip_package);
    void *decode_package(std::vector<video_frame> &frames, size_t &package_size);
    video_encoder_config *config();
    ~video_encoder();
};