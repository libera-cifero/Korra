#include "config/parser/frame_codec/cipher/cipher_parser.hpp"
#include "config/data/cipher_config.hpp"
#include "config/parser/parser.hpp"
#include "video_codec/frame_codec/cipher/cipher.hpp"

cipher* cipher_parser::parse(json j){
    string t = j["type"];

    for(auto p : specific_parsers){
        if(p->type() == t){
            if(auto in = dynamic_cast<contexted_in<cipher_config>*>(p)) in -> context_in = context_in;
            return p -> parse(j[t + "Settings"]);
        }
    }

    throw runtime_error(std::format("Can't parse cipher! Unknown type \"{}!\"", t));

}

json cipher_parser::serialize(cipher *c){
    for(auto parser : specific_parsers){
        if(parser -> can_serialize(c)){
            if(auto in = dynamic_cast<contexted_in<cipher_config>*>(parser)) in -> context_in = context_in;
            string type = parser->type();
            json obj=json::object({{"type", type}});
            obj[type + "Settings"] = parser->serialize(c);
            return obj;
        }
    }
    throw runtime_error("Can't serialize cipher!");
}