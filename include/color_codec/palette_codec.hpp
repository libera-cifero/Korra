#pragma once
#include "color_codec.hpp"

class palette_codec : public color_codec
{
private:
    int *_palette;
protected:
    virtual float $get_distance(int color0, int color1) = 0;
public:
    palette_codec(int *palette, int bits_per_number);
    virtual int color_to_number(int color) override;
    virtual int number_to_color(int number) override;
    ~palette_codec();
};