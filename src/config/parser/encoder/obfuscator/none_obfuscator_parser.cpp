#include "config/parser/encoder/obfuscator/none_obfuscator_parser.hpp"
#include "config/parser/parser.hpp"
#include "video/encoder/obfuscator/obfuscator.hpp"
#include "video/encoder/obfuscator/none_obfuscator.hpp"

string none_obfuscator_parser::type(){ return "none"; }

obfuscator* none_obfuscator_parser::parse(json j){
    return new none_obfuscator;
}

json none_obfuscator_parser::serialize(obfuscator*){
    return json::object({{"type", type()}});
}