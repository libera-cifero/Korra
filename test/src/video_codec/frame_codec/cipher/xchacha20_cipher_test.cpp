#include "video_codec/frame_codec/cipher/xchacha20_cipher.hpp"
#include "test.hpp"
#include <algorithm>
#include <random>
#include <string>
#include <vector>

using random_bytes_engine = std::independent_bits_engine<std::default_random_engine, CHAR_BIT, byte>;

byte *get_random_key(){
    int size = xchacha20_cipher::KEY_SIZE;
    byte *key = new byte[size];
    random_bytes_engine engine;
    std::generate_n(key, size, std::ref(engine));

    return key;
}

byte *get_random_iv(){
    int size = xchacha20_cipher::NONCE_SIZE;
    byte *iv = new byte[size];
    random_bytes_engine engine;
    std::generate_n(iv, size, std::ref(engine));

    return iv;
}

bool strings_are_equaled(char *str_a, char *str_b, int length){
    for(int i = 0; i < length; i++){
        if(str_a[i] != str_b[i]) return false;
    }

    return true;
}

void test_encode_decode() {
    const char *test_name = "xchacha20_cipher_test.test_encode_decode";
    printInfo(test_name);
    xchacha20_config *config = new xchacha20_config;
    config -> key = get_random_key();
    config -> iv = get_random_iv();
    config -> encrypted_size = 140;
    auto cipher = new xchacha20_cipher(config);

    std::vector<std::string> test_cases = { 
        "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean m",
        "Sed ut perspiciatis unde omnis iste natus error sit voluptatem accusantium doloremque laudantium, to",
        "But I must explain to you how all this mistaken idea of denouncing pleasure and praising pain was bo",
        "Li Europan lingues es membres del sam familie. Lor separat existentie es un myth. Por scientie, musi",
        "Far far away, behind the word mountains, far from the countries Vokalia and Consonantia, there live."
    };

    for(int i = 0; i < test_cases.size(); i++){
        char *encrypted, *decrypted;
        try{
            char *payload = test_cases[i].data();
            encrypted = cipher->encrypt(payload);
            decrypted = cipher->decrypt(encrypted);

            if(!strings_are_equaled(payload, decrypted, cipher->payload_size())){
                delete cipher;
                delete encrypted;
                delete decrypted;
                fail(test_name, "encrypted and decrypted aren't equals");
            }
        }
        catch(...){
            if(encrypted != nullptr) delete encrypted;
            if(decrypted != nullptr) delete decrypted;
            delete cipher;
            fail(test_name, "something went wrong with test_cases[%d]!", 1, i);
        }

        delete [] encrypted;
        delete [] decrypted;
    }
    
    delete cipher;
    printPass(test_name);
}

int main(){
    test_encode_decode();
    return 0;
}