#include "color_codec/codec_json.hpp"
#include "color_codec/hsv_codec.hpp"
#include "color_codec/rgb_palette_codec.hpp"
#include "color_codec/ycbcr_palette_codec.hpp"
#include <regex>
#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

static int parse_int(string int_str) {
    char *end;
    long byte = strtol(int_str.c_str(), &end, 10);
    if(*end != '\0'){
        throw runtime_error("Invalid number " + int_str + "!");
    }
    return byte;
}

static int parse_byte(string byte_str){
    int number=parse_int(byte_str);
    if(number < 0 || number > 255) throw runtime_error("Invalid byte " + byte_str + "!");
    return number;
}

static int ycbcr_to_rgb(int y, int cb, int cr)
{
    int r = static_cast<int>(y + 1.402 * (cr - 128));
    int g = static_cast<int>(y - 0.344136 * (cb - 128) - 0.714136 * (cr - 128));
    int b = static_cast<int>(y + 1.772 * (cb - 128));

    r = clamp(r, 0, 255);
    g = clamp(g, 0, 255);
    b = clamp(b, 0, 255);

    return (r << 16) | (g << 8) | b;
}

static int parse_color(string color){
    const regex regex_hex("#(([0-F]|[0-f]){6})");
    const regex regex_rgb("rgb\\(\\s*([0-9]{1,3}),\\s*([0-9]{1,3}),\\s*([0-9]{1,3})\\s*\\)");
    const regex regex_ycbcr("ycbcr\\(\\s*([0-9]{1,3}),\\s*([0-9]{1,3}),\\s*([0-9]{1,3})\\s*\\)");
    
    smatch match_hex, match_rgb, match_ycbcr;
    if(regex_match(color, match_hex, regex_hex)){
        int color = strtol(match_hex[1].str().c_str(), nullptr, 16);
        return color;
    }
    else if(regex_match(color, match_rgb, regex_rgb)){
        string r_str = match_rgb[1].str(), g_str = match_rgb[2].str(), b_str = match_rgb[3].str();
        int r = parse_byte(r_str), g = parse_byte(g_str), b = parse_byte(b_str);
        return (r<<16)|(g<<8)|b;
    }
    else if(regex_match(color, match_ycbcr, regex_ycbcr)) {
        string y_str = match_ycbcr[1].str(), cb_str = match_ycbcr[2].str(), cr_str = match_ycbcr[3].str();
        int y = parse_int(y_str), cb = parse_int(cb_str), cr = parse_int(cr_str);
        return ycbcr_to_rgb(y, cb, cr);
    }
    else{
        throw runtime_error("Invalid color_function!");
    }
}

static vector<int> parse_palette(json &settingsObject) {
    vector<string> palette_str = settingsObject["palette"].get<vector<string>>();
    vector<int> palette;
    for(string color_str : palette_str){
        int color = parse_color(color_str);
        palette.push_back(color);
    }

    return palette;
}

color_codec *parse_color_codec(json &root){
    string color_mode = root["colorCodec"].get<string>();
    json settingsObject = root[color_mode + "Settings"];
    int bits_per_number = settingsObject["bitsPerNumber"].get<int>();
    if(color_mode == "hsv"){
        return new hsv_codec(bits_per_number);
    }
    else if(color_mode == "rgbPalette"){
        vector<int> palette = parse_palette(settingsObject);
        return new rgb_palette_codec(palette.begin().base(), bits_per_number);
    }
    else if(color_mode == "ycbcrPalette"){
        vector<int> palette = parse_palette(settingsObject);
        return new ycbcr_palette_codec(palette.begin().base(), bits_per_number);
    }

    throw runtime_error("Invalid colorCodec \"" + color_mode + "\"!");
}

json serialize_color_codec(color_codec *codec) {
    json root = json::object(), settings = json::object();
    settings["bitsPerNumber"] = codec->bits_per_number();
    string color_codec;

    if(dynamic_cast<hsv_codec*>(codec)) {
        color_codec = "hsv";
    }
    else if(auto *rgb_codec = dynamic_cast<palette_codec*>(codec)) {
        int *palette = rgb_codec->palette();
        vector<string> palette_list(rgb_codec->max_number() + 1);
        
        settings["palette"] = palette_list;
        delete [] palette;
        if(dynamic_cast<rgb_palette_codec*>(codec)) {
            color_codec = "rgbPalette";
        }
        else if(dynamic_cast<ycbcr_palette_codec*>(codec)) {
            color_codec = "ycbcrPalette";
        }
        else {
            throw runtime_error("Unknown codec!");
        }
    }
    else{
        throw runtime_error("Unknown codec!");
    }
    root["type"] = color_codec;
    root[color_codec + "Settings"] = settings;

    return root;
}