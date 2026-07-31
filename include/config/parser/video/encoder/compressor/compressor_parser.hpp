#include "config/parser/parser.hpp"
#include "video_encoder/compressor/compressor.hpp"
class compressor_parser : public abstract_parser<compressor*> {
public:
    compressor* parse(json) override;
    json serialize(compressor*) override;
};