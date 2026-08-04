#pragma once
#include "block_codec/block_codec.hpp"
#include "config/data/mosaic_config.hpp"
#include <stdint.h>

struct mosaic_settings : public mosaic_config {
    block_codec *codec;
};