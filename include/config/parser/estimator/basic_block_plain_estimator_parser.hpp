#include "estimator_parser.hpp"

class mosaic_plain_estimator_parser : public specific_parser<estimator*> {
public:
    string type() override;
    estimator* parse(json) override;
    json serialize(estimator*) override;
};