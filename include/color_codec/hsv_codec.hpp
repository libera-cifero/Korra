#include "color_codec.hpp"

class hsv_codec : public color_codec
{
private:
    const int COLOR_palette = 1536;
    int _get_index_by_color(int color);
    int _get_color_by_index(int color_index);

    int _number_to_index(int number, int color_bit_resolution);
    int _index_to_number(int number, int color_bit_resolution);
public:
    hsv_codec(int bits_per_number);
    virtual int color_to_number(int color) override;
    virtual int number_to_color(int number) override;
};