#include "data_boxer/data_boxer.hpp"
#include "data_boxer/data/korra_data.hpp"
#include "data_boxer/data/piecable_data.hpp"
#include "data_boxer/data/begin_label_data.hpp"
#include "data_boxer/data/pieced_data.hpp"
#include "video_codec/payload_storage.hpp"
#include <cstdio>
#include <cstring>
#include <stdexcept>

data_boxer::data_boxer(payload_storage *storage){
    _storage = storage;
}

void data_boxer::_increment_data_count(){
    _data_frame_counter++;
    memcpy(_current_payload, &_data_frame_counter, 2);
}

char *data_boxer::_put_conitinioused_data(korra_data *data) {
    char *end = data->to_bytes(_current_payload + _storage -> payload_index);
    _increment_data_count();
    _storage -> payload_index = end - _current_payload;
    return end;
}

void data_boxer::_begin_new_payload(){
    _data_frame_counter = 0;
    _storage->begin_new_payload();
    _current_payload = _storage->current_payload();
}

int data_boxer::_get_free_space(){
    return _storage->payload_size() - _storage->payload_index - 1;
}

bool data_boxer::_is_case_A(korra_data *data){
    return _get_free_space() >= data->total_size();
}

bool data_boxer::_is_case_B(korra_data *data){
    bool is_piecable = dynamic_cast<piecable_data*>(data) != nullptr;
    return !is_piecable && _get_free_space() < data->total_size();
}

void data_boxer::_box_case_A(korra_data *data) {
    _put_conitinioused_data(data);
    if(_storage->payload_index >= _storage->payload_size() - 1){
        _data_frame_counter = 0;
        _begin_new_payload();
    }
}

void data_boxer::_box_case_B(korra_data *data) {
    _begin_new_payload();
    if(data->total_size() > _storage->payload_size()){
        char error_buff[256];
        sprintf(error_buff, "Invalid data size! data's type: %d, data's total_size: %d, frame's payload size %d!\n", data->type(), data->total_size(), _storage->payload_size());
        throw runtime_error(error_buff);
    }
    _box_case_A(data);
}

void data_boxer::_box_case_C(korra_data *data) {
    begin_label_data *label = new begin_label_data(data -> type(), data -> size());
    if(_is_case_A(label)) _box_case_A(label);
    else _box_case_B(label);

    int data_size = data->size();
    char *piece_buffer = new char[data->total_size()];
    data->to_bytes(piece_buffer);
    while(data_size > 0){
        int max_size = _get_free_space() - pieced_data::MIN_TOTAL_SIZE;
        int piece_data_size = data_size > max_size ? max_size : data_size;

        pieced_data *piece = new pieced_data(label -> id(), piece_data_size, piece_buffer);
        _box_case_A(piece);
        delete piece;

        piece_buffer += piece_data_size;
        data_size -= piece_data_size;
    }

    delete label;
}

void data_boxer::box(korra_data *data) {
    _current_payload = _storage->current_payload();
    if(_is_case_A(data)) _box_case_A(data);
    else if(_is_case_B(data)) _box_case_B(data);
    else _box_case_C(data);
}

data_boxer::~data_boxer() { }