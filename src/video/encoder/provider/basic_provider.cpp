#include "video/encoder/provider/basic_provider.hpp"
#include "video/encoder/provider/basic_block/basic_block_math.hpp"
#include <cstddef>
#include <cstdint>
#include <cstring>

basic_provider::basic_provider(basic_block_settings *settings) {
    _settings = settings;
    _frame = alloc_by_config(*_settings);
    _container = new basic_block_container(_frame, _settings);
    int block_count = get_block_count(settings->frame_width, settings->frame_height, settings->block_size);
    _payload_size = get_basic_block_frame_payload_size(settings->codec->bits_per_number(), block_count);
    _frame_size = settings->frame_width * settings->frame_height * 3;
}

size_t basic_provider::frame_size() {
    return _frame_size;
}

size_t basic_provider::payload_size() {
    return _payload_size;
}

char* basic_provider::to_payload(char* frame) {
    uint8_t *payload = new uint8_t[_payload_size];
    _container->read(_container->begin(), payload, _payload_size);

    return reinterpret_cast<char*>(payload);
}

char* basic_provider::to_frame(char* data) {
    _container->write(_container->begin(), reinterpret_cast<uint8_t*>(data), _payload_size);

    char *frame = new char[_frame_size];
    memcpy(frame, _frame, _frame_size);

    memset(_frame, 0, _frame_size);

    return frame;
}

basic_provider::~basic_provider() {
    delete _settings;
    delete _container;
}