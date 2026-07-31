#include "data_boxer/data_boxer.hpp"
#include "data_boxer/data/korra_data.hpp"
#include "data_boxer/data/piecable_data.hpp"
#include "data_boxer/data/begin_label_data.hpp"
#include "video_encoder/payload_storage.hpp"

data_boxer::data_boxer(payload_storage *storage){
    _storage = storage;
}

void data_boxer::_put_conitinioused_data(korra_data *data, int total_size) {

}

char *data_boxer::_box_case_A(korra_data *data, int total_size, int free_count) {
}

char *data_boxer::_box_case_B(korra_data *data, int total_size, int free_count) {

}

char *data_boxer::_box_case_C(piecable_data *data, int total_size, int free_count) {
    
}

void data_boxer::box(korra_data *data) {
    
}

data_boxer::~data_boxer() {
    delete _storage;
}