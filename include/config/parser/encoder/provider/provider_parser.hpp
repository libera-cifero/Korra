#include "encoder/provider/provider.hpp"
#include "config/parser/parser.hpp"

class provider_parser : public json_parser<provider*>{
public:
    virtual provider* parse(json) override;
    virtual json serialize(provider*) override;
};