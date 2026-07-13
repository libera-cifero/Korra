#include "encoder/provider/len_reader/data_length_reader.hpp"
#include "config/parser/parser.hpp"

class data_length_reader_parser : public json_parser<data_length_reader*>{
public:
    virtual data_length_reader* parse(json) override;
    virtual json serialize(data_length_reader*) override;
};