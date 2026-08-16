#include "config/data/cipher_config.hpp"
#include "video_codec/frame_codec/cipher/xchacha20_cipher.hpp"
#include "config/parser/frame_codec/cipher/xchacha20_cipher_parser.hpp"
#include "test.hpp"
#include "io.hpp"
#include "string_utils.hpp"
#include <exception>

void test_parse_with_password(){
    const char *test_name = "xchacha20_cipher_parser_test.test_parse_with_password";
    printInfo(test_name);

    cipher_config context;
    context.frame_width = 1280;
    context.frame_height = 720;
    context.fps = 30;
    context.encrypted_size = 140;
    xchacha20_cipher_parser parser;
    parser.context_in = context;
    cipher *data = nullptr;
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
        fail(test_name, "something went wrong!");
    }
}

void test_parse_with_keyfile(){
    const char *test_name = "xchacha20_cipher_parser_test.test_parse_with_keyfile";
    printInfo(test_name);

    cipher_config context;
    context.frame_width = 1280;
    context.frame_height = 720;
    context.fps = 30;
    context.encrypted_size = 140;
    xchacha20_cipher_parser parser;
    parser.context_in = context;
    cipher *data = nullptr;
    vector<string> keyfiles = {"keyfile0.key", "keyfile1.key", "keyfile2.key"};
    try{
        for(auto keyfile : keyfiles){
            path p = DATA_PATH / "xchacha20" / to_native_path(keyfile);
            data = parser.parse(json({{"keyFile", p.string()}}));

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
        }
        
        printPass(test_name);
    }
    catch(exception &e){
        if(data != nullptr) delete data;
        fail(test_name, e.what());
    }
    catch(...){
        if(data != nullptr) delete data;
        fail(test_name, "something went wrong!");
    }
}

int main(){
    test_parse_with_password();
    test_parse_with_keyfile();
    return 0;
}