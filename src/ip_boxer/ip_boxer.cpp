#include "ip_boxer/ip_boxer.hpp"
#include <cstring>
#include <functional>
#include <mutex>
#include <vector>

ip_boxer::ip_boxer(int max_sending_size, int timeout){
    _max_sending_size = max_sending_size;
    _timeout = timeout;
}

char *ip_boxer::_pop_bytes_for_boxing(){
    char *data = new char[_max_sending_size];
    memset(data, 0, _max_sending_size);
    int send_total_count = _package_send_pieces.size();
    int pop_count = send_total_count > _max_sending_size ? _max_sending_size : send_total_count;
    
    for(int i = 0; i < pop_count; i++) {
        data[i] = _package_send_pieces.front();
        _package_send_pieces.pop_front();
    }

    return data;
}

void ip_boxer::_setup_timer() {
    _timer = new timer(_timeout);
    _timer->on_timeout([&](){
        lock_guard<mutex> lock(_send_mutex);
        char *data = _pop_bytes_for_boxing();
        for(auto event : _timeout_events) event(data);
    });
    _timer->launch();
}

template<typename T> 
void ip_boxer::_clear_list(vector<T*> &list, function<void(T*)> deleter) {
    for(auto elem : list) deleter(elem);
    list.clear();
}

void ip_boxer::box(korra_data *data, vector<char*> &frame_payloads_buffer) {
    _clear_list<char>(frame_payloads_buffer, [](char *data){ delete [] data; });
    if(_timer == nullptr) _setup_timer();

    char *bytes_to_send = data->to_bytes();
    int size = data->size();
    for(int i = 0; i < size; i++) _package_send_pieces.push_back(bytes_to_send[i]);
    delete [] bytes_to_send;
    
    _timer->reset();
    lock_guard<mutex> lock(_send_mutex);
    while(_package_send_pieces.size() >= _max_sending_size) {
        char *data = _pop_bytes_for_boxing();
        frame_payloads_buffer.push_back(data);
    }
}

void ip_boxer::unbox(char *decoded_data, vector<korra_data*> &buffer) {
    _clear_list<korra_data>(buffer, [](korra_data *data){ delete data; });
    for(int i = 0; i < _max_sending_size; i++) _package_receive_pieces.push_back(decoded_data[i]);
}

void ip_boxer::on_timeout(function<void(char*)> event){
    _timeout_events.push_back(event);
}

ip_boxer::~ip_boxer(){
    if(_timer != nullptr) delete _timer;
}