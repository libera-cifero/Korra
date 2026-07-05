#include "palette_codec.hpp"

class ycbcr_palette_codec : public palette_codec {
protected:
    virtual float $get_distance(int color0, int color1) override;
public:
    ycbcr_palette_codec(int *palette, int bits_per_number);
};