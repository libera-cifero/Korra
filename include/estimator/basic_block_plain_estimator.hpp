#pragma once
#include "estimator.hpp"
#include "video/encoder/provider/mosaic/mosaic_settings.hpp"

class mosaic_plain_estimator : public estimator {
private:
    int _frame_count_total;
    int _frame_payload_capacity;
    int _blocks_per_frame;
    int _bits_per_number;
public:
    mosaic_plain_estimator(mosaic_settings *settings);
    float put_package(void*) override;
};