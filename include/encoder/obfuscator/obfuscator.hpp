#pragma once
class obfuscator {
public:
    virtual char *obfuscate(char *data, int *data_length) = 0;
    virtual char *extract(char *obfuscated, int data_length) = 0;
};