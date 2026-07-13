#include "config/parser/encoder/provider/provider_parser.hpp"
#include "encoder/provider/provider.hpp"

class basic_provider_parser : public provider_parser {
public:
    provider* parse(json) override;
    json serialize(provider*) override;
};