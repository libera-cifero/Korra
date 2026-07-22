#include "video/encoder/provider/basic_provider.hpp"
#include <cstddef>
#include <cstdint>
#include <cstring>

basic_provider::basic_provider(basic_block_settings *settings, data_length_reader *reader) : provider(reader) {
    _settings = settings;
    _frame = alloc_by_config(*_settings);
    _container = new basic_block_container(_frame, _settings);
}

vector<video_frame>* basic_provider::to_frames(char *data) {
    auto frames = new vector<video_frame>;
    
    uint8_t *ptr = reinterpret_cast<uint8_t*>(data);

    int data_count;
    memcpy(&data_count, data, 4);

    while(data_count > 0) {
        size_t container_byte_count = _container -> byte_count();
        size_t write_count = data_count > container_byte_count ? container_byte_count : data_count;
        _container->write(_container->begin(), ptr, write_count);
        ptr += write_count;
        data_count -= write_count;
        uint8_t *data = new uint8_t[container_byte_count];
        memcpy(data, _frame, container_byte_count);
        frames->push_back({
            .data = data,
            .size = container_byte_count
        });
    }

    return frames;
}

char* basic_provider::to_data(vector<video_frame> *frames) {
    int byte_count;
    _container->read(_container->begin(), reinterpret_cast<uint8_t*>(&byte_count), 4);
    
}


basic_provider::~basic_provider() {
    delete _settings;
    delete _container;
}