#pragma once
#include "video/encoder/provider/provider.hpp"
#include "video/encoder/corrector/corrector.hpp"
#include "video/encoder/obfuscator/obfuscator.hpp"

struct encoder_config {
    obfuscator *obfuscator;
    provider *provider;
    corrector *corrector;
};