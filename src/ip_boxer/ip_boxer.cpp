#include "ip_boxer/ip_boxer.hpp"
#include <cstring>
#include <functional>
#include <mutex>
#include <vector>

ip_boxer::ip_boxer(int max_sending_size, int timeout){
    _max_sending_size = max_sending_size;
    _timeout = timeout;
}

char *ip_boxer::_pop_data_to_boxing(){
    char *data = new char[_max_sending_size];
    memset(data, 0, _max_sending_size);
    int send_total_count = _package_send_pieces.size();
    int pop_count = send_total_count > _max_sending_size ? _max_sending_size : send_total_count;
    
    for(int i = 0; i < pop_count; i++) {
        data[i] = _package_send_pieces.front();
        _package_send_pieces.pop();
    }

    return data;
}

void ip_boxer::_setup_timer() {
    _timer = new timer(_timeout);
    _timer->on_timeout([&](){
        lock_guard<mutex> lock(_send_mutex);
        char *data = _pop_data_to_boxing();
        on_timeout(data);
    });
    _timer->launch();
}

void ip_boxer::box(char *ip_package, vector<char*> &frame_payloads_buffer) {
    frame_payloads_buffer.clear();
    if(_timer == nullptr) _setup_timer();

    uint16_t package_size;
    memcpy(&package_size, ip_package + 2, 2);
    for(int i = 0; i < package_size; i++) _package_send_pieces.push(ip_package[i]);
    
    _timer->reset();
    lock_guard<mutex> lock(_send_mutex);
    while(_package_send_pieces.size() >= _max_sending_size) {
        char *data = _pop_data_to_boxing();
        frame_payloads_buffer.push_back(data);
    }
}

void ip_boxer::unbox(char *decoded_data, vector<char*> &buffer){
    buffer.clear();
    
}

void ip_boxer::on_timeout(char *payload_to_send) {

}

ip_boxer::~ip_boxer(){
    if(_timer != nullptr) delete _timer;
}