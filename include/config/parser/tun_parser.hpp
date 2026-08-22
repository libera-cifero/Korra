#pragma once
#include "tun/tun.hpp"
#include "parser.hpp"

class tun_parser : public json_parser<tun*> {
private:
    uint32_t _parse_ip_address(string address);
    bool _check_address(uint32_t ip, int mask);
public:
    tun *parse(json) override;
    json serialize(tun *) override;
};