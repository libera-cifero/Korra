#pragma once
#include "tun/tun.hpp"
#include "parser.hpp"

class tun_parser : public json_parser<tun*> {
public:
    tun *parse(json) override;
    json serialize(tun *) override;
};