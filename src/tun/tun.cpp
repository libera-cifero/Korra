#include "tun/tun.hpp"
#include "lib/log.hpp"
#include <spdlog/spdlog.h>
#include <cstdint>
#include <format>
#include <string>
#ifdef _WIN32
    #include <winsock2.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <arpa/inet.h>
#endif


tun::tun(string &tun_name, string &ip, uint8_t subnet_mask) {
    _ip = ip;
    _subnet_mask = subnet_mask;
    _name = tun_name;
    _is_header_reading = true;
    _readed_count = _package_read_count = 0;
}

string tun::ip(){ return _ip; }

uint8_t tun::subnet_mask(){
    return _subnet_mask;
}

string tun::ip_with_mask() {
    return format("{}/{}",_ip, _subnet_mask);
}

string tun::name(){
    return _name;
}

void tun::init_buffer(){
    _read_buffer = new char[mtu()];
}

char *tun::_current_read_buffer(){ return _read_buffer + _readed_count; }

char *tun::read(){
    char *buff = _current_read_buffer();
    if(_is_header_reading){
        int recv_count = __read(buff, 20 - _readed_count);
        if(recv_count < 0) return nullptr;
        spdlog::debug("something was recieved! count={}", recv_count);
        _readed_count += recv_count;
        _is_header_reading = _readed_count < 20;
        memcpy(&_package_read_count, _read_buffer + 2, 2);
        _package_read_count = ntohs(_package_read_count);
        if(_is_header_reading) return nullptr;
    }
    buff = _current_read_buffer();
    int recv_count = __read(buff, _package_read_count - _readed_count);
    if(recv_count < 0) return nullptr;
    spdlog::debug("something was recieved! count={}", recv_count);
    _readed_count += recv_count;
    if(_readed_count >= _package_read_count){
        char *ip_package = new char[_package_read_count];
        memcpy(ip_package, _read_buffer, _package_read_count);
        _is_header_reading = true;
        _readed_count = _package_read_count = 0;
        return ip_package;
    }

    return nullptr;
}

void tun::write(char *ip_package){
    auto prefix = get_method_prefix("tun.write");
    uint16_t size;
    memcpy(&size, ip_package + 2, 2);
    int n = __write(ip_package, size);
    spdlog::info("{} wrote {}/{} bytes!", prefix, n, size);
    spdlog::info("{} completed!", prefix);
}

tun::~tun(){
    delete [] _read_buffer;
}