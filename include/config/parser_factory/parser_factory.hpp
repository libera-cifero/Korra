#include "../parser/parser.hpp"
#include "../data/root_config.hpp"

class parser_factory {
public:
    virtual json_parser<root_config>* build() = 0;
};