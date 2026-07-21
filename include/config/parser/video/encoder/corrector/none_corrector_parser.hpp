#include "config/parser/parser.hpp"
#include "video/encoder/corrector/corrector.hpp"
class none_corrector_parser : public specific_parser<corrector*> {
public:
    string type() override;
    corrector* parse(json) override;
    json serialize(corrector*) override;
};