#include "tun/linux_tun.hpp"
#include "lib/log.hpp"
#include <asm-generic/ioctls.h>
#include <cstdint>
#include <fcntl.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <netinet/in.h>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>

linux_tun::linux_tun(string &tun_name, string &ip, uint8_t subnet_mask) : tun(tun_name, ip, subnet_mask) {
    struct ifreq ifr;
    if((_file_descriptor = open("/dev/net/tun", O_RDWR)) == -1) throw runtime_error("/dev/net/tun open error!");

    int flags = fcntl(_file_descriptor, F_GETFL, 0);
    fcntl(_file_descriptor, F_SETFL, flags | O_NONBLOCK);

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

int linux_tun::__read(char *buffer, int count){
    return ::read(_file_descriptor, buffer, count);
}

int linux_tun::__write(char *buffer, int count) {
    return ::write(_file_descriptor, buffer, count);
}

int linux_tun::mtu() { return _mtu; }

linux_tun::~linux_tun(){
    auto prefix = get_method_prefix("linux_tun.~linux_tun");
    spdlog::debug("{} destructing...", prefix);
    close(_file_descriptor);
    spdlog::debug("{} linux_tun was destructed!", prefix);
}