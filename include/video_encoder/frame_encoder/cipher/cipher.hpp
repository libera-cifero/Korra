#pragma once
class cipher {
private:
    int _payload_size;
public:
    cipher(int payload_size);
    int payload_size();

    virtual char *encrypt(char *data) = 0;
    virtual char *decrypt(char *encrypted) = 0;
    virtual ~cipher();
};