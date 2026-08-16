#include "tun/linux_tun.hpp"
#include <cstdint>
#include <fcntl.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstring>

linux_tun::linux_tun(string &tun_name, string &ip, uint8_t subnet_mask) : tun(ip, subnet_mask) {
    _tun_name = tun_name;

    struct ifreq ifr;
    _file_descriptor = open("/dev/net/tun", O_RDWR);

    memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_flags = IFF_TUN | IFF_NO_PI;
    strncpy(ifr.ifr_name, tun_name.c_str(), IFNAMSIZ);

    ioctl(_file_descriptor, TUNSETIFF, (void *)&ifr);
}

char *linux_tun::read(){
    char *buffer = new char[65535];
    int n = ::read(_file_descriptor, buffer, 65535);
    if (n < 0) {
        delete[] buffer;
        return nullptr;
    }

    return buffer;
}

void linux_tun::write(char *ip_package) {
    uint16_t size;
    memcpy(&size, ip_package + 2, 2);
    ::write(_file_descriptor, ip_package, size);
}

linux_tun::~linux_tun(){
    close(_file_descriptor);
}