#include "tun/tun_builder.hpp"
#include "video_socket.hpp"

#ifdef __linux__ 
#include "tun/linux_tun.hpp"
#else
#error "Unsupported OS"
#endif

tun *build_tun(tun_config &config){
    tun *t;
#ifdef __linux__
    t = new linux_tun(config.name);
#else
    return nullptr;
#endif
    t->init();
    return t;
}