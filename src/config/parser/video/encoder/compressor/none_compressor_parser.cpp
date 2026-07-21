#include "config/parser/video/encoder/compressor/none_compressor_parser.hpp"
#include "config/parser/parser.hpp"
#include "video/encoder/compressor/compressor.hpp"
#include "video/encoder/compressor/none_compressor.hpp"

string none_compressor_parser::type(){ return "none"; }

compressor* none_compressor_parser::parse(json j){
    return new none_compressor;
}

json none_compressor_parser::serialize(compressor*){
    return json::object({{"type", type()}});
}