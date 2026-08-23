#include "tun/tun_builder.hpp"
#include "video_socket.hpp"

#ifdef __linux__ 
#include "tun/linux_tun.hpp"
#else
#error "Unsupported OS"
#endif

tun *build_tun(tun_config &config){
    #ifdef __linux__
    return new linux_tun(config.name, config.ip, config.mask);
    #endif

    return nullptr;
}