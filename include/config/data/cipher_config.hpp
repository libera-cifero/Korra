#pragma once
#include "video_config.hpp"

struct cipher_config : public video_config {
    int encrypted_size;
};