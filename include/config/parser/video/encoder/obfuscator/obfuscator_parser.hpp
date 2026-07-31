#include "config/parser/parser.hpp"
#include "video_encoder/obfuscator/obfuscator.hpp"
class obfuscator_parser : public abstract_parser<obfuscator*> {
public:
    obfuscator* parse(json) override;
    json serialize(obfuscator*) override;
};