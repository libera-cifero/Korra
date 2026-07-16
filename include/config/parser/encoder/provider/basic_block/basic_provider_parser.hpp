#pragma once
#include "encoder/provider/provider.hpp"
#include "config/parser/parser.hpp"

class basic_provider_parser : public json_parser<provider*> {
public:
    provider* parse(json) override;
    json serialize(provider*) override;
};