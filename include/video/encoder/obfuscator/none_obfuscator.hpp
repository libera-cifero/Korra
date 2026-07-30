#include "obfuscator.hpp"

class none_obfuscator : public obfuscator {
public:
    char *obfuscate(char *data) override;
    char *deobfuscate(char *obfuscated) override;
};