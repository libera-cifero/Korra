#include "config/parser/frame_codec/cipher/none_cipher_parser.hpp"
#include "config/parser/parser.hpp"
#include "video_codec/frame_codec/cipher/cipher.hpp"
#include "video_codec/frame_codec/cipher/none_cipher.hpp"

string none_cipher_parser::type(){
    return "none";
}

bool none_cipher_parser::can_serialize(cipher *c){
    return dynamic_cast<cipher*>(c);
}

cipher *none_cipher_parser::parse(json j){
    return new none_cipher(context_in.payload_size);
}

json none_cipher_parser::serialize(cipher *c){
    return json::object();
}