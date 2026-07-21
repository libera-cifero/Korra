#pragma once
#include "compressor/compressor.hpp"
#include "obfuscator/obfuscator.hpp"
#include "corrector/corrector.hpp"
#include "provider/provider.hpp"
#include "video_frame.hpp"
#include <cstddef>
#include <vector>

using namespace std;
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
    vector<video_frame>* encode_bytes(void *bytes, size_t bytes_size);
    void *decode_bytes(vector<video_frame> &frames, size_t &bytes_size);
    video_encoder_config *config();
    ~video_encoder();
};