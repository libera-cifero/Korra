#include "config/parser/parser.hpp"
#include "video_encoder/obfuscator/obfuscator.hpp"
class none_obfuscator_parser : public specific_parser<obfuscator*> {
public:
    string type() override;
    obfuscator* parse(json) override;
    json serialize(obfuscator*) override;
};