#include "tun/tun.hpp"
#include <cstdint>
#include <format>
#include <string>

tun::tun(string &ip, uint8_t subnet_mask) {
    _ip = ip;
    _subnet_mask = subnet_mask;
}

string tun::ip(){ return _ip; }

uint8_t tun::subnet_mask(){
    return _subnet_mask;
}

string tun::ip_with_mask() {
    return format("{}/{}",_ip, _subnet_mask);
}