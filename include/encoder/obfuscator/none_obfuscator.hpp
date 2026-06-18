#include "obfuscator.hpp"

class none_obfuscator : public obfuscator {
public:
    char *obfuscate(char *data, int *data_length) override;
    char *extract(char *obfuscated, int data_length) override;
};