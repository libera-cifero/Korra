#include "config/parser/parser.hpp"
#include "video_encoder/compressor/compressor.hpp"
class none_compressor_parser : public specific_parser<compressor*> {
public:
    string type() override;
    compressor* parse(json) override;
    json serialize(compressor*) override;
};