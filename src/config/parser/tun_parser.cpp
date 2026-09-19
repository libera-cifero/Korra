#include "config/parser/tun_parser.hpp"
#include "config/parser/parser.hpp"
#include "tun/tun_builder.hpp"
#include "video_socket.hpp"
#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>
#ifdef _WIN32
    #include <winsock2.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <arpa/inet.h>
#endif


bool tun_parser::_check_address(uint32_t ip){
    const int count = 3;
    uint32_t available_nets[count] = { 0xC0A80100, 0xAC100000, 0x0A000000 };//192.168.1.0, 172.16.0.0, 10.0.0.0
    uint32_t net_masks[count] = { 24, 16, 8 };
    for(int i = 0; i < count; i++){
        uint32_t mask_val = (1 << net_masks[i]) - 1;
        uint32_t net = ip & mask_val;
        uint32_t available_net = ntohl(available_nets[i]);
        if(net == available_net) return true;
    }

    return false;
}
tun *tun_parser::parse(json j){
    tun_config config;
    if(j.contains("address") && j["address"].is_string()){
        string ip = j["address"];
        uint32_t ip_val;
        if(!inet_pton(AF_INET, ip.c_str(), &ip_val) || !_check_address(ip_val)) throw runtime_error(format("Invalid IP {}!",ip));
        config.ip = ip;
    }
    else throw runtime_error("tun_parser.parse: property \"name\" is undefined or not string!");

    return build_tun(config);
}

json tun_parser::serialize(tun *t) {
    return json::object({
        { "address", t->ip() }
    });
}