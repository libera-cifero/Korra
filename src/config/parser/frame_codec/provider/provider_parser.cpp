#include "config/parser/frame_codec/provider/provider_parser.hpp"
#include "config/data/video_config.hpp"
#include "config/parser/parser.hpp"
#include "video_codec/frame_codec/provider/provider.hpp"
#include <format>
#include <stdexcept>

provider *provider_parser::parse(json obj){
    string t = obj["type"];

    for(auto p : specific_parsers){
        if(p->type() == t){
            if(auto in = dynamic_cast<contexted_in<video_config>*>(p)) in -> context_in = context_in;
            return p -> parse(obj[t + "Settings"]);
        }
    }

    throw runtime_error(std::format("Can't parse provider! Unknown type \"{}!\"", t));
}

json provider_parser::serialize(provider *p){
    for(auto parser : specific_parsers){
        if(parser -> can_serialize(p)){
            if(auto in = dynamic_cast<contexted_in<video_config>*>(parser)) in -> context_in = context_in;
            string type = parser->type();
            json obj=json::object({{"type", type}});
            obj[type + "Settings"] = parser->serialize(p);
            return obj;
        }
    }
    throw runtime_error("Can't serialize provider!");
}