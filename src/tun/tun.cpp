#include "tun/tun.hpp"
#include "lib/log.hpp"
#include <cstring>
#include <spdlog/spdlog.h>
#include <cstdint>
#include <format>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#ifdef _WIN32
    #include <winsock2.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <arpa/inet.h>
#endif


tun::tun(string &ip) {
    _ip = ip;
    _ip_val = 0;
    if(!inet_pton(AF_INET, _ip.c_str(), &_ip_val)) throw runtime_error(format("Invalid IP {}!", _ip));
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

void tun::init(){
    __init_properties(_ip, &_name, &_subnet_mask);
    _read_buffer = new char[mtu()];
}

char *tun::_current_read_buffer(){ return _read_buffer + _readed_count; }

bool tun::_must_transmit(){
    int src_ip;
    memcpy(&src_ip, _read_buffer + 12, 4);
    int mask_val = (1 << _subnet_mask) - 1;
    bool self_ip = src_ip == _ip_val;
    bool different_networks = (src_ip & mask_val) != (_ip_val & mask_val);
    return self_ip || different_networks;
}

char *tun::read(){
    char *buff = _current_read_buffer();
    int recv_count = __read(buff, mtu());
    if(recv_count < 0) return nullptr;
    memcpy(&_package_read_count, _read_buffer + 2, 2);
    _package_read_count = ntohs(_package_read_count);
    buff = _current_read_buffer();
    if(is_debug_level()){
        string prefix = get_method_prefix("tun.read");
        spdlog::debug("{} something was recieved! count={}", prefix, recv_count);
    }
    _readed_count += recv_count;
    if(_readed_count >= _package_read_count){
        char *ip_package = nullptr;
        if(_must_transmit()){
            ip_package = new char[_package_read_count];
            memcpy(ip_package, _read_buffer, _package_read_count);
        }
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