#pragma once
#include "tun/tun.hpp"
#include "parser.hpp"

class tun_parser : public json_parser<tun*> {
private:
    bool _check_address(uint32_t ip);
public:
    tun *parse(json) override;
    json serialize(tun *) override;
};