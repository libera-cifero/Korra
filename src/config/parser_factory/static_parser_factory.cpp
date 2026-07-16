#include "config/parser_factory/static_parser_factory.hpp"
#include "config/parser/root_parser.hpp"

json_parser<root_config>* static_parser_factory::build(){
    auto parser = new root_parser;
    
    return parser;
}