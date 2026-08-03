#pragma once
#include "video_codec/frame_codec/provider/provider.hpp"
#include "video_codec/corrector/corrector.hpp"
#include "video_codec/cipher/cipher.hpp"

struct encoder_config {
    cipher *cipher;
    provider *provider;
    corrector *corrector;
};