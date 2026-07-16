#pragma once
#include "config/parser/encoder/provider/len_reader/len_reader_parser.hpp"
#include "encoder/provider/provider.hpp"
#include "config/parser/parser.hpp"

class provider_parser : public abstract_parser<provider*>{
public:
    data_length_reader_parser *length_reader;

    provider_parser();

    provider* parse(json) override;
    json serialize(provider*) override;
};