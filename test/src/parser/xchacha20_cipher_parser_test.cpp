#include "config/data/cipher_config.hpp"
#include "video_codec/frame_codec/cipher/xchacha20_cipher.hpp"
#include "config/parser/frame_codec/cipher/xchacha20_cipher_parser.hpp"
#include "test.hpp"
#include "string_utils.hpp"
#include <cstring>

void test_parse(){
    const char *test_name = "xchacha20_cipher_parser_test.test_parse";
    printInfo(test_name);

    cipher_config context;
    context.frame_width = 1280;
    context.frame_height = 720;
    context.fps = 30;
    context.encrypted_size = 140;
    xchacha20_cipher_parser parser;
    parser.context_in = context;
    cipher *data;
    try{
        data = parser.parse(json({{"password", "123456"}}));

        string test_case = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean m";
        char *encrypted = data->encrypt(test_case.data());
        char *decrypted = data->decrypt(encrypted);

        if(!strings_are_equaled(decrypted, test_case.data(), data->payload_size())){
            delete data;
            fail(test_name, "decrypted and input data don't equal!");
        }

        delete [] encrypted;
        delete [] decrypted;
        delete data;
        printPass(test_name);
    }
    catch(...){
        if(data != nullptr) delete data;
        fail(test_name, "something gone wrong!");
    }
}

int main(){
    test_parse();
    return 0;
}