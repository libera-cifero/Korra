#pragma once
#include "estimator.hpp"
#include "video/encoder/provider/basic_block/basic_block_settings.hpp"

class basic_block_plain_estimator : public estimator {
private:
    int _frame_count_total;
    int _frame_payload_capacity;
    int _blocks_per_frame;
    int _bits_per_number;
public:
    basic_block_plain_estimator(basic_block_settings *settings);
    float put_package(void*) override;
};