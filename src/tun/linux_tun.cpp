#include "tun/linux_tun.hpp"
#include "lib/log.hpp"
#include <cstdint>
#include <fcntl.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstring>

linux_tun::linux_tun(string &tun_name, string &ip, uint8_t subnet_mask) : tun(ip, subnet_mask) {
    _tun_name = tun_name;

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
    _mtu = ifr.ifr_mtu;
}

char *linux_tun::read(){
    auto prefix = get_method_prefix("linux_tun.read");
    spdlog::info("{} reading...", prefix);
    char *buffer = new char[65535];
    int n = ::read(_file_descriptor, buffer, 65535);
    spdlog::info("{} readed {} bytes!", prefix, n);
    if(is_debug_level()){
        spdlog::debug("{} data={}", prefix, get_byte_str(buffer));
    }
    if (n < 0) {
        delete[] buffer;
        return nullptr;
    }
    spdlog::info("{} completed!", prefix);
    return buffer;
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
    close(_file_descriptor);
}