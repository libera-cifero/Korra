#pragma once
#include "../data/root_config.hpp"
#include "config/parser/encoder/provider/provider_parser.hpp"
#include "config/parser/pipes_parser.hpp"
#include "parser.hpp"

class root_parser : public json_parser<root_config>{
public:
    provider_parser *provider;
    pipes_parser *pipes;

    root_parser();
    root_config parse(json) override;
    json serialize(root_config) override;
};