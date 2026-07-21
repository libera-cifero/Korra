#pragma once
#include "estimator.hpp"

class basic_block_plain_estimator : public estimator {
public:
    basic_block_plain_estimator(int width, int height, int block_size);
    float estimate(std::vector<video_frame>& frames, size_t payload_bytes) override;
};