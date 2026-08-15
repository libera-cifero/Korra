#include "config/parser/frame_codec/cipher/xchacha20_cipher_parser.hpp"
#include "config/data/cipher_config.hpp"
#include "config/parser/parser.hpp"
#include "video_codec/frame_codec/cipher/xchacha20_cipher.hpp"
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>
#include <cstring>
#include <format>
#include <fstream>
#include <ios>
#include <sstream>
#include <stdexcept>
#include <regex>
#include <string>
#include <random>

string xchacha20_cipher_parser::type(){
    return "xchacha20";
}

bool xchacha20_cipher_parser::can_serialize(cipher *c){
    return dynamic_cast<xchacha20_cipher*>(c);
}

crypto_byte *xchacha20_cipher_parser::_read_hex(string hex_str, int byte_size){
    string pattern = format("[0-F]{{{}}}", byte_size * 2);
    regex expression(pattern);
    if(!regex_match(hex_str,expression))
        throw runtime_error(format("xchacha20_cipher_parser._read_hex: {} is not matched by pattern {}!", hex_str, pattern));
    
    auto buffer = new crypto_byte[byte_size];
    for(int i = 0; i < byte_size; i++) {
        int j = i * 2;
        char s0 = hex_str[j], s1 = hex_str[j + 1];
        char x = 0;
        x |= s1 >= 'A' ? s1 - 'A' : s1 - '0';
        x |= (s0 >= 'A' ? s0 - 'A' : s0 - '0') << 4;
        buffer[i] = x;
    }

    return buffer;
}

int xchacha20_cipher_parser::_get_seed(crypto_byte *key){
    int seed = 0;
    for(int i = 0; i < xchacha20_cipher::KEY_SIZE; i++) seed += key[i];

    return seed;
}

int xchacha20_cipher_parser::_rand_int(int seed) {
    std::random_device dev;
    std::seed_seq seq{ seed };
    std::mt19937 random(seq);
    return random();
}

crypto_byte *xchacha20_cipher_parser::_generate_key(string password){
    CryptoPP::SHA256 hash;
    crypto_byte *key = new crypto_byte[xchacha20_cipher::KEY_SIZE];
    crypto_byte *pswd_bytes = (crypto_byte*)password.c_str();
    hash.CalculateDigest(key, pswd_bytes, password.length());

    int seed = _get_seed(key);
    int count = _rand_int(seed) % 5000;
    for(int i = 0; i < count; i++) hash.CalculateDigest(key, key, xchacha20_cipher::KEY_SIZE);
    
    return key;
}

crypto_byte *xchacha20_cipher_parser::_generate_iv(crypto_byte *key) {
    int seed = _get_seed(key);
    int count = _rand_int(seed) % 2376;
    std::random_device dev;
    std::seed_seq seq{ seed };
    std::mt19937 random(seq);

    CryptoPP::SHA256 hash;
    crypto_byte *iv = new crypto_byte[24], *hash_bytes = new crypto_byte[32];
    memcpy(hash_bytes, key, 32);
    for(int i = 0; i < count; i++) {
        for(int j = 0; j < count; j++) hash.CalculateDigest(hash_bytes, hash_bytes, 32);
    }

    memcpy(iv, hash_bytes + 4, 24);
    delete [] hash_bytes;
    return iv;
}

cipher *xchacha20_cipher_parser::parse(json j){
    crypto_byte *key, *iv;
    if(j["key"].is_string()){
        if(!j["iv"].is_string())
            throw runtime_error("xchacha20_cipher_parser.parse: field iv must be defined and string!");
        key = _read_hex(j["key"], 32);
        iv = _read_hex(j["iv"], 24);
    }
    else if(j["keyFile"].is_string()){
        string key_str, iv_str;
        {
            fstream file(j["keyFile"], ios_base::in);
            stringstream stream;
            stream << file.rdbuf();
            std::getline(stream, key_str, '\n');
            std::getline(stream, iv_str, '\n');
            file.close();
        }
        key = _read_hex(key_str, 32);
        iv = _read_hex(iv_str, 24);
    }
    else if(j["password"].is_string()){
        string password = j["password"];
        key = _generate_key(password);
        iv = _generate_iv(key);
    }
    else throw runtime_error("xchacha20_cipher_parser.parse: key, keyFile and password are undefined!");

    xchacha20_settings config;
    memcpy((cipher_config*)&config, &context_in, sizeof(cipher_config));
    config.key = key;
    config.iv = iv;
    return new xchacha20_cipher(config);
}

string xchacha20_cipher_parser::_write_hex(crypto_byte *bytes, int bytes_count) {
    string str;
    for(int i = 0; i < bytes_count; i++){
        crypto_byte b = bytes[i];
        crypto_byte b0 = b >> 4, b1 = b & 0x0F;
        char s0 = b0 >= 0xA ? b0 - 0xA + 'A' : b0 + '0';
        char s1 = b1 >= 0xA ? b1 - 0xA + 'A' : b1 + '0';
        str+=s0;
        str+=s1;
    }

    return str;
}

json xchacha20_cipher_parser::serialize(cipher *c){
    auto ciph = static_cast<xchacha20_cipher*>(c);
    string key_str = _write_hex(ciph -> key(), ciph -> KEY_SIZE);
    string iv_str = _write_hex(ciph -> iv(), ciph -> NONCE_SIZE);
    return json({{"key", key_str}, {"iv", iv_str}});
}