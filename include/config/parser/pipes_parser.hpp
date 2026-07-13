#include "config/data/pipes_config.hpp"
#include "config/parser/parser.hpp"

class pipes_parser : public json_parser<pipes_config>{
public:
    pipes_config parse(json) override;
    json serialize(pipes_config) override;
};