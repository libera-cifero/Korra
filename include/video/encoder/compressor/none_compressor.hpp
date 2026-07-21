#include "compressor.hpp"

class none_compressor: public compressor {
public:
    char *compress(char *data, int *data_length) override;
    char *decompress(char *data, int data_length) override;
};