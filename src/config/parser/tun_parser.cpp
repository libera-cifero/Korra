#include "config/parser/tun_parser.hpp"
#include "config/parser/parser.hpp"
#include "lib/CLI11.hpp"
#include "tun/tun_builder.hpp"
#include "video_socket.hpp"
#include <cstdint>
#include <cstdlib>
#include <format>
#include <netinet/in.h>
#include <regex>
#include <stdexcept>

uint32_t tun_parser::_parse_ip_address(string address){
    auto strs = CLI::detail::split(address, '.');
    uint32_t ip_val = 0;
    uint8_t *ip = (uint8_t*)&ip_val;
    for(int i = 0; i < 4; i++){
        int byte = atoi(strs[i].c_str());
        if(byte > 255)
            throw runtime_error(format("tun_parser._parse_ip_address: invalid IP \"{}\"!", address));
        ip[3-i] = byte; //because big-endian
    }

    return ntohl(ip_val);
}

bool tun_parser::_check_address(uint32_t ip, int mask){
    if(mask > 32) throw runtime_error(format("tun_parser._check_address: invalid mask {}", mask));
    const int count = 3;
    uint32_t available_nets[count] = { 0xC0A80100, 0xAC100000, 0x0A000000 };//192.168.1.0, 172.16.0.0, 10.0.0.0
    uint32_t net_masks[count] = { 24, 16, 8 };
    for(int i = 0; i < count; i++){
        uint32_t mask_val = (1 << net_masks[i]) - 1;
        uint32_t net = ip & mask_val;
        uint32_t available_net = ntohl(available_nets[i]);
        if(net == available_net && net != ip) return true;
    }

    return false;
}

tun *tun_parser::parse(json j){
    tun_config config;
    if(j.contains("name") && j["name"].is_string()) config.name = j["name"];
    else throw runtime_error("tun_parser.parse: property \"name\" is undefined or not string!");
    if(j.contains("address") && j["address"].is_string()){
        string pattern = "((\\d{1,3}\\.){3}\\d{1,3})\\/(\\d{1,2})";
        regex address_check(pattern);
        string raw_address = j["address"];
        smatch result;
        if(!regex_match(raw_address, result, address_check)) 
            throw runtime_error(format("tun_parser.parse: property \"address\" with value \"{}\" doesn't match by pattern {}", raw_address, pattern));
        
        string address_str = result[1];
        int mask = atoi(result[3].str().c_str());
        uint32_t ipv4 = _parse_ip_address(address_str);
        bool addres_correct = _check_address(ipv4, mask);
        if(!addres_correct) 
            throw runtime_error(format("tun_parser.parse: invalid address \"{}\"!\nAllowed nets: 172.16.0.0/12, 192.168.0.0/16, 10.0.0.0/24",raw_address));
        config.ip = address_str;
        config.mask = mask;
    }
    else throw runtime_error("tun_parser.parse: property \"address\" is undefined or not string!");

    return build_tun(config);
}

json tun_parser::serialize(tun *t) {
    return json::object({
        { "name", t->name() },
        { "address", t->ip_with_mask() }
    });
}