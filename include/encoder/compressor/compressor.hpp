#pragma once
class compressor {
public:
    virtual char *compress(char *data, int *data_length) = 0;
    virtual char *decompress(char *data, int data_length) = 0;
};