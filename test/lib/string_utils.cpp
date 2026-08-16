#include "string_utils.hpp"

bool strings_are_equaled(char *str_a, char *str_b, int length){
    for(int i = 0; i < length; i++){
        if(str_a[i] != str_b[i]) return false;
    }

    return true;
}