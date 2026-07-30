#pragma once
class obfuscator {
public:
    int payload_size();
    int frame_size();

    virtual char *obfuscate(char *data) = 0;
    virtual char *deobfuscate(char *obfuscated) = 0;
    virtual ~obfuscator() { };
};