#include "config/parser/parser.hpp"
#include "estimator/estimator.hpp"

class estimator_parser : public abstract_parser<estimator*> {
public:
    estimator* parse(json) override;
    json serialize(estimator*) override;
};