#pragma once
#include "video_encoder/frame_encoder/provider/provider.hpp"
#include "video_encoder/corrector/corrector.hpp"
#include "video_encoder/obfuscator/obfuscator.hpp"

struct encoder_config {
    obfuscator *obfuscator;
    provider *provider;
    corrector *corrector;
};