#include "data_boxer/data_unboxer.hpp"
#include "data_boxer/data/ip_data.hpp"
#include "data_boxer/data/korra_data.hpp"
#include "data_boxer/data/piecable_data.hpp"
#include "data_boxer/data/pieced_data.hpp"
#include "data_boxer/data/begin_label_data.hpp"
#include <concepts>
#include <cstdint>
#include <cstring>

template<std::derived_from<korra_data> T>
static korra_data *build_data(){
    korra_data *data = new T;
    return data;
}

data_unboxer::data_unboxer(){    
    _data_builders = { 
        { ip_data().type(), build_data<ip_data> },
        { begin_label_data().type(), build_data<begin_label_data> },
        { pieced_data().type(), build_data<pieced_data> }
    };
}

int data_unboxer::_select_by_id(int id) {
    for(int i = 0; i < _builiding_data.size(); i++){
        if(_builiding_data[i]->id() == id) return i;
    }

    //throw new runtime_error(format("data with id {} not found!", id));
    return -1;
}

void data_unboxer::_put_begin_label(begin_label_data *label){
    auto new_data = (piecable_data*)_data_builders[label->payload_type()]();
    new_data->set_id(label->id());
    new_data->init_buffer(label -> data_size());
    _builiding_data.push_back(new_data);
}

void data_unboxer::_put_pieced(pieced_data *piece){
    int index = _select_by_id(piece->id());
    if(index > -1) {
        piecable_data *container = _builiding_data[index];
        container -> put_piece(piece -> piece(), piece -> data_size());
        if(container -> is_built()) {
            _built_data.push_back(container);
            _builiding_data.erase(_builiding_data.begin() + index);
        }
    }
}

void data_unboxer::put_payload(char *payload, int payload_size){
    uint16_t count;
    memcpy(&count, payload, 2);
    payload += 2;
    for(int i = 0; i < count; i++) {
        uint16_t type;
        memcpy(&type, payload, 2);
        korra_data *data = _data_builders[type]();
        payload = data -> from_bytes(payload);

        if(auto label = dynamic_cast<begin_label_data*>(data)){
            _put_begin_label(label);
            delete data;
        }
        else if(auto piece = dynamic_cast<pieced_data*>(data)){
            _put_pieced(piece);
            delete [] piece->piece();
            delete data;
        }
        else if(dynamic_cast<ip_data*>(data) != nullptr){
            _built_data.push_back(data);
        }
    }
}

korra_data *data_unboxer::unbox(){
    if(_built_data.empty()) return nullptr;
    korra_data *data = _built_data.front();
    _built_data.erase(_built_data.begin());
    return data;
}