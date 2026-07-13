#include "encoder/provider/provider.hpp"
#include "encoder/compressor/compressor.hpp"
#include "encoder/corrector/corrector.hpp"
#include "encoder/obfuscator/obfuscator.hpp"

struct encoder_config {
    obfuscator *obfuscator;
    provider *provider;
    corrector *corrector;
    compressor *compressor;
};