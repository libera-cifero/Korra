#pragma once
class color_codec
{
private:
    int _bits_per_number;
public:
    color_codec(int bits_per_number);
    int bits_per_number();
    int max_number();
    virtual int color_to_number(int color);
    virtual int number_to_color(int number);
    virtual ~color_codec() { }
};