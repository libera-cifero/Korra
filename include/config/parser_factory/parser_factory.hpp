#include "../parser/parser.hpp"

template<typename T>
class parser_factory {
public:
    virtual json_parser<T>* build() = 0;
};