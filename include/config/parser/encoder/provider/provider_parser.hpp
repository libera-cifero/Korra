#pragma once
#include "encoder/provider/provider.hpp"
#include "config/parser/parser.hpp"

class provider_parser : public abstract_parser<provider*>{
public:
    provider* parse(json) override;
    json serialize(provider*) override;
};