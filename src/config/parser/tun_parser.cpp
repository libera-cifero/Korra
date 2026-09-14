#include "config/parser/tun_parser.hpp"
#include "config/parser/parser.hpp"
#include "lib/CLI11.hpp"
#include "tun/tun_builder.hpp"
#include "video_socket.hpp"
#include <cstdint>
#include <cstdlib>
#include <format>
#include <netinet/in.h>
#include <stdexcept>

tun *tun_parser::parse(json j){
    tun_config config;
    if(j.contains("name") && j["name"].is_string()) config.name = j["name"];
    else throw runtime_error("tun_parser.parse: property \"name\" is undefined or not string!");

    return build_tun(config);
}

json tun_parser::serialize(tun *t) {
    return json::object({
        { "name", t->name() },
        { "address", t->ip_with_mask() }
    });
}