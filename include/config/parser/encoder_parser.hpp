#include "config/data/encoder_config.hpp"
#include "config/parser/parser.hpp"

class encoder_parser : public json_parser<encoder_config>{
public:
    encoder_config parse(json) override;
    json serialize(encoder_config) override;
};