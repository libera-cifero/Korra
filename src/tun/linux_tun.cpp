#include "tun/linux_tun.hpp"
#include "lib/log.hpp"
#include <asm-generic/ioctls.h>
#include <cstdint>
#include <fcntl.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>

linux_tun::linux_tun(string &tun_name, string &ip, uint8_t subnet_mask) : tun(ip, subnet_mask) {

    struct ifreq ifr;
    if((_file_descriptor = open("/dev/net/tun", O_RDWR)) == -1) throw runtime_error("/dev/net/tun open error!");

    memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_flags = IFF_TUN | IFF_NO_PI;
    strncpy(ifr.ifr_name, tun_name.c_str(), IFNAMSIZ);

    if(ioctl(_file_descriptor, TUNSETIFF, &ifr) == -1){
        close(_file_descriptor);
        throw runtime_error("ioctl TUNSETIFF");
    }
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock == -1) throw runtime_error("socket creation error!");
    if(ioctl(sock, SIOCGIFMTU, &ifr) == -1){
        close(_file_descriptor);
        close(sock);
        throw runtime_error("ioctl SIOCGIFMTU");
    }
    close(sock);
    ioctl(_file_descriptor, FIONBIO, 1);

    _mtu = ifr.ifr_mtu;
    _read_buffer = new char[_mtu];
    _tun_name = tun_name;
    _readed_count = _package_read_count = 0;
    _is_header_reading = true;
}

char *linux_tun::_current_read_buffer(){ return _read_buffer + _readed_count; }

char *linux_tun::read(){
    char *buff = _current_read_buffer();
    if(_is_header_reading){
        int recv_count = recv(_file_descriptor, buff, 20, 0);
        if(recv_count < 0) return nullptr;
        _readed_count += recv_count;
        _is_header_reading = _readed_count < 20;
        memcpy(&_package_read_count, _read_buffer + 2, 2);
        if(_is_header_reading) return nullptr;
    }
    buff = _current_read_buffer();
    int recv_count = recv(_file_descriptor, buff, _package_read_count, 0);
    if(recv_count < 0) return nullptr;
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

void linux_tun::write(char *ip_package) {
    auto prefix = get_method_prefix("linux_tun.write");
    uint16_t size;
    memcpy(&size, ip_package + 2, 2);
    int n = ::write(_file_descriptor, ip_package, size);
    spdlog::info("{} wrote {}/{} bytes!", prefix, n, size);
    spdlog::info("{} completed!", prefix);
}

int linux_tun::mtu() { return _mtu; }

linux_tun::~linux_tun(){
    delete [] _read_buffer;
    close(_file_descriptor);
}