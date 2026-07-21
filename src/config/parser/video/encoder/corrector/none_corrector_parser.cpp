#include "config/parser/video/encoder/corrector/none_corrector_parser.hpp"
#include "config/parser/parser.hpp"
#include "video/encoder/corrector/corrector.hpp"
#include "video/encoder/corrector/none_corrector.hpp"

string none_corrector_parser::type(){ return "none"; }

corrector* none_corrector_parser::parse(json j){
    return new none_corrector;
}

json none_corrector_parser::serialize(corrector*){
    return json::object({{"type", type()}});
}