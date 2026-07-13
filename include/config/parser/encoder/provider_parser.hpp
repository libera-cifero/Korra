#include "encoder/provider/provider.hpp"
#include "../parser.hpp"

provider *parse_config(json);
json serialize_config(provider*);