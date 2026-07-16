#include "config/parser/root_parser.hpp"
#include "config/parser/encoder/provider/provider_parser.hpp"
#include "config/parser/pipes_parser.hpp"

root_parser::root_parser(){
    provider = new provider_parser;
    pipes = new pipes_parser;
}