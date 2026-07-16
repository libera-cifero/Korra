#include "parser_factory.hpp"

class static_parser_factory : public parser_factory {
public:
    json_parser<root_config>* build() override;
};