#include "config/parser/parser.hpp"
#include "encoder/corrector/corrector.hpp"
class corrector_parser : public abstract_parser<corrector*> {
public:
    corrector* parse(json) override;
    json serialize(corrector*) override;
};
