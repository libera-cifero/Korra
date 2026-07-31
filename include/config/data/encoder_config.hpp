#pragma once
#include "video_encoder/frame_encoder/provider/provider.hpp"
#include "video_encoder/corrector/corrector.hpp"
#include "video_encoder/cipher/cipher.hpp"

struct encoder_config {
    cipher *cipher;
    provider *provider;
    corrector *corrector;
};