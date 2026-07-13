#include "json.hpp"
using json = nlohmann::json;

template<typename T>
class json_parser {
public:
    virtual T parse(json) = 0;
    virtual json serialize(T) = 0;
};