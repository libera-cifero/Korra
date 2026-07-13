#include "../data/root_config.hpp"
#include "parser.hpp"

class root_parser : public json_parser<root_config>{
public:
    root_config parse(json) override;
    json serialize(root_config) override;
};