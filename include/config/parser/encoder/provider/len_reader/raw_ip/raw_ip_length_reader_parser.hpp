#include "../len_reader_parser.hpp"

class raw_ip_length_reader_parse : public data_length_reader_parser {
public:
    data_length_reader* parse(json) override;
    json serialize(data_length_reader*) override;
};