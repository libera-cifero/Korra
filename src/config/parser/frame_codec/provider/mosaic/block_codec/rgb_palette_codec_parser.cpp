#include "config/parser/frame_codec/provider/mosaic/block_codec/rgb_palette_codec_parser.hpp"
#include "config/data/block_codec_config.hpp"
#include "config/data/palette_codec_config.hpp"
#include "config/parser/parser.hpp"
#include "video_codec/frame_codec/provider/mosaic/block_codec/block_codec.hpp"
#include "video_codec/frame_codec/provider/mosaic/block_codec/rgb_palette_codec.hpp"
#include "lib/color.hpp"
#include <cstring>
#include <regex>

string rgb_palette_codec_parser::type(){
    return "rgbPalette";
}

bool rgb_palette_codec_parser::can_serialize(block_codec *codec){
    return dynamic_cast<rgb_palette_codec*>(codec) != nullptr;
}

static int parse_byte(string byte_str){
    char *end;
    long number = strtol(byte_str.c_str(), &end, 10);
    if(*end != '\0'){
        throw runtime_error("Invalid number " + byte_str + "!");
    }
    if(number < 0 || number > 255) throw runtime_error("Invalid byte " + byte_str + "!");
    return number;
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
    else{
        throw runtime_error("Invalid color_function!");
    }
}

static int *parse_rgb_palette(json &settingsObject) {
    vector<string> palette_str = settingsObject["palette"].get<vector<string>>();
    int *palette = new int[palette_str.size()];
    for(int i = 0; i < palette_str.size(); i++) 
        palette[i] = parse_color(palette_str[i]);
    
    return palette;
}

block_codec *rgb_palette_codec_parser::parse(json j){
    palette_codec_config<int> config;
    memcpy((block_codec_config*)&config, &context_in, sizeof(block_codec_config));
    config.palette = parse_rgb_palette(j);
    return new rgb_palette_codec(config);
}

json rgb_palette_codec_parser::serialize(block_codec *c){
    rgb_palette_codec *codec = static_cast<rgb_palette_codec*>(c);
    vector<string> colors;
    int *palette = codec->palette();
    for(int i = 0; i < codec->numbers_count(); i++){
        colors.push_back(rgb_to_hex(palette[i]));
    }
    json j=json::object({{"palette", colors}});
    return j;
}