#include "block_codec_json.hpp"
#include "video_codec/frame_codec/provider/mosaic/block_codec/rgb_palette_codec.hpp"
#include "lib/color.hpp"
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

static int *parse_rgb_palette(json &settingsObject) {
    vector<string> palette_str = settingsObject["palette"].get<vector<string>>();
    int *palette = new int[palette_str.size()];
    for(int i = 0; i < palette_str.size(); i++){
        string color_str = palette_str[i];
        palette[i] = parse_color(color_str);
    }

    return palette;
}

block_codec *parse_block_codec(json &root){
    string codec_type = root["type"].get<string>();
    json settingsObject = root[codec_type + "Settings"];
    int bits_per_number = settingsObject["bitsPerNumber"].get<int>();

    if(codec_type == "rgbPalette"){
        palette_codec_config<int> config;
        config.frame_width = settingsObject["frameWidth"].get<int>();
        config.frame_height = settingsObject["frameHeight"].get<int>();
        config.palette = parse_rgb_palette(settingsObject);
        return new rgb_palette_codec(config);
    }

    throw runtime_error("Invalid colorCodec \"" + codec_type + "\"!");
}

json serialize_block_codec(block_codec *codec) {
    json root = json::object(), settings = json::object();
    settings["bitsPerNumber"] = codec->bits_per_number();
    string codec_type;
    
    if(auto *rgb_codec = dynamic_cast<rgb_palette_codec*>(codec)) {
        int *palette = rgb_codec->palette();
        int count = rgb_codec->color_count();
        vector<string> palette_list(count);
        for(int i = 0; i < count; i++) palette_list[i] = rgb_to_hex(palette[i]);
        
        settings["palette"] = palette_list;
        delete [] palette;
        codec_type = "rgbPaletteCodec";
    }
    else{
        throw runtime_error("Unknown codec!");
    }
    root["type"] = codec_type;
    root[codec_type + "Settings"] = settings;

    return root;
}