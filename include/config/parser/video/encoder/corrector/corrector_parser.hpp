#include "config/parser/parser.hpp"
#include "video_codec/corrector/corrector.hpp"
class corrector_parser : public abstract_parser<corrector*> {
public:
    corrector* parse(json) override;
    json serialize(corrector*) override;
};
