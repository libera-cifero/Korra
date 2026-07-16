#pragma once
#include "encoder/provider/len_reader/data_length_reader.hpp"
#include "config/parser/parser.hpp"

class raw_ip_length_reader_parse : public json_parser<data_length_reader*> {
public:
    data_length_reader* parse(json) override;
    json serialize(data_length_reader*) override;
};