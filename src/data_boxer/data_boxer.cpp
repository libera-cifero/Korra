#include "data_boxer/data_boxer.hpp"
#include "data_boxer/data/korra_data.hpp"
#include "data_boxer/data/piecable_data.hpp"
#include "data_boxer/data/begin_label_data.hpp"
#include "data_boxer/data/pieced_data.hpp"
#include <cstring>
#include <stdexcept>

data_boxer::data_boxer(int payload_size_per_frame){
    _payload_size_per_frame = payload_size_per_frame;
    _begin_new_frame();
}

int data_boxer::_last_frame_index(){
    int size = _payload_buffer.size();
    if(size == 0) return 0;
    return (size - 1) / _payload_size_per_frame;
}

void data_boxer::_increment_data_frame_counter(){
    _data_frame_counter++;
    int frame_begin = _last_frame_index() * _payload_size_per_frame;
    char *bin_count = (char*)&_data_frame_counter;
    _payload_buffer[frame_begin]=bin_count[0];
    _payload_buffer[frame_begin + 1] = bin_count[1];
}

int data_boxer::_free_bytes_count(){
    return _payload_buffer.size() % _payload_size_per_frame;
}

void data_boxer::_begin_new_frame(){
    int prev_free_count = _free_bytes_count();
    //I append 2 for _data_count bytes also
    for(int i = 0; i < prev_free_count + 2; i++) _payload_buffer.push_back(0);

    _data_frame_counter = 0;
}

char *data_boxer::pop_frame_payload() {
    char *payload = new char[_payload_size_per_frame];
    memset(payload, 0, _payload_size_per_frame);
    for(int i = 0; i < _payload_size_per_frame && !_payload_buffer.empty(); i++){
        payload[i] = _payload_buffer.front();
        _payload_buffer.pop_front();
    }
    if(_payload_buffer.empty()){
        _payload_buffer.push_front(0);
        _payload_buffer.push_front(0);
        _data_frame_counter = 0;
    }
    return payload;
}

void data_boxer::_put_conitinioused_data(korra_data *data, int total_size){
    char *bin_data = data->to_bytes();
    for(int i = 0; i < total_size; i++) _payload_buffer.push_back(bin_data[i]);
    delete [] bin_data;
    _increment_data_frame_counter();
}

char *data_boxer::_box_case_A(korra_data *data, int total_size, int free_count) {
    _put_conitinioused_data(data, total_size);
    if(total_size == free_count){
        char *payload = pop_frame_payload();
        return payload;
    }
    return nullptr;
}

char *data_boxer::_box_case_B(korra_data *data, int total_size, int free_count) {
    //- 2, because first 2 bytes are used for _data_frame_counter
    int max_size = _payload_size_per_frame - 2;

    if(total_size > max_size) 
        throw std::runtime_error(std::format("Data is too large to box!\nSize of data {}, when max size for frame is {}", total_size, max_size));
    
    char *payload = pop_frame_payload();
    _begin_new_frame();
    _put_conitinioused_data(data, total_size);
    return payload;
}

char *data_boxer::_box_case_C(piecable_data *data, int total_size, int free_count) {
    begin_label_data *label = new begin_label_data(data->type(), data->size());
    free_count -= label->size();
    if(free_count < 0) {
        _begin_new_frame();
        free_count = _free_bytes_count();
    }
    _put_conitinioused_data(label, total_size);
    int data_size = label->data_size();
    char *bytes = data->to_bytes();
    char *ptr = bytes + 2; //+2 to skip type in bytes
    int id = label->id();
    while(data_size > 0){
        if(free_count <= pieced_data::MIN_TOTAL_SIZE){
            _begin_new_frame();
            free_count = _free_bytes_count();
        }
        int piece_size = free_count - pieced_data::MIN_TOTAL_SIZE;
        if(piece_size > data_size)
            piece_size = data_size;
        pieced_data *piece = new pieced_data(id, piece_size, ptr);
        _put_conitinioused_data(piece, piece -> total_size());
        free_count = _free_bytes_count();
        data_size -= piece_size;
    }
    
    delete [] bytes;
}

char* data_boxer::box(korra_data *data) {
    int free_count = _free_bytes_count();
    int total_size = data->total_size();
    _timer->reset();
    if(total_size <= free_count) 
        return _box_case_A(data, total_size, free_count);
    

    auto piecable = dynamic_cast<piecable_data*>(data);
    if(piecable == nullptr)
        return _box_case_B(data, total_size, free_count);
    return _box_case_C(piecable, total_size, free_count);
}

data_boxer::~data_boxer() {
    if(_timer != nullptr) delete _timer;
}