#include "estimator/mosaic_plain_estimator.hpp"
#include "video/encoder/provider/mosaic/mosaic_settings.hpp"
#include "video/encoder/provider/mosaic/mosaic_math.hpp"

mosaic_plain_estimator::mosaic_plain_estimator(mosaic_settings *settings) : estimator() {
    _bits_per_number = settings->codec->bits_per_number();
    _blocks_per_frame = get_block_count(settings->frame_width, settings->frame_height, settings->block_size);
    _frame_payload_capacity  = get_mosaic_frame_payload_size(_bits_per_number, _blocks_per_frame);
}

float mosaic_plain_estimator::put_package(void *ip_package) {
    estimator::put_package(ip_package);

    int block_count = get_block_count($payload_byte_count_total, _bits_per_number);
    int frame_count = block_count / _blocks_per_frame + (block_count % _blocks_per_frame ? 1 : 0);
    int max_payload = frame_count * _frame_payload_capacity;
    return 100.0 * $payload_byte_count_total / max_payload;
}