#include "config/parser/video_codec/cipher/none_cipher_parser.hpp"
#include "config/parser/parser.hpp"
#include "video_codec/cipher/cipher.hpp"
#include "video_codec/cipher/none_cipher.hpp"

string none_cipher_parser::type(){ return "none"; }

cipher* none_cipher_parser::parse(json j){
    return new none_cipher;
}

json none_cipher_parser::serialize(cipher*){
    return json::object({{"type", type()}});
}