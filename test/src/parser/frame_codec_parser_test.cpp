#include "config/data/cipher_config.hpp"
#include "config/data/mosaic_config.hpp"
#include "config/data/palette_codec_config.hpp"
#include "config/data/video_config.hpp"
#include "config/parser/frame_codec/cipher/cipher_parser.hpp"
#include "config/parser/frame_codec/cipher/none_cipher_parser.hpp"
#include "config/parser/frame_codec/frame_codec_parser.hpp"
#include "config/parser/frame_codec/provider/mosaic/block_codec/block_codec_parser.hpp"
#include "config/parser/frame_codec/provider/mosaic/block_codec/rgb_palette_codec_parser.hpp"
#include "config/parser/frame_codec/provider/mosaic/mosaic_provider_parser.hpp"
#include "config/parser/frame_codec/provider/provider_parser.hpp"
#include "config/parser/parser.hpp"
#include "config/parser/video_config_parser.hpp"
#include "io.hpp"
#include "test.hpp"
#include "video_codec/frame_codec/cipher/none_cipher.hpp"
#include "video_codec/frame_codec/frame_codec.hpp"
#include "video_codec/frame_codec/provider/mosaic/block_codec/rgb_palette_codec.hpp"
#include "video_codec/frame_codec/provider/mosaic/mosaic_settings.hpp"
#include "video_codec/frame_codec/provider/mosaic_provider.hpp"
#include "video_codec/frame_codec/provider/provider.hpp"
#include <cstring>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

frame_codec_parser *set_up(){
    provider_parser *p = new provider_parser;
    block_codec_parser *block = new block_codec_parser;
    block->specific_parsers.push_back(new rgb_palette_codec_parser);
    mosaic_provider_parser *mosaic = new mosaic_provider_parser(block);
    p->specific_parsers = { mosaic };
    cipher_parser *c = new cipher_parser;
    c->specific_parsers.push_back(new none_cipher_parser);
    frame_codec_parser *parser = new frame_codec_parser(p, c);
    return parser;
}

map<string, frame_codec*> get_test_cases(){
    map<string, frame_codec*> cases;
    video_config context0 {
        .frame_width = 1920,
        .frame_height = 1080,
        .fps = 60
    };
    mosaic_settings *settings0 = new mosaic_settings;
    palette_codec_config<int> config0;
    config0.bits_per_number = 5;
    config0.block_size = 16;
    config0.palette = new int[32] {
        0x000000,
        0xF8F8F8,
        0x0078F8,
        0x78F800,
        0xF80078,
        0x909090,
        0x00F880,
        0x8000F8,
        0xF88000,
        0x009808,
        0x080098,
        0x980800,
        0x60F8F8,
        0xF860F8,
        0xF8F860,
        0x484848,
        0x008888,
        0x7070F8,
        0x70F870,
        0x880088,
        0x888800,
        0xF87070,
        0x00E0E8,
        0xE0E800,
        0xE800E0,
        0x1818F8,
        0x18F818,
        0xF81818,
        0xB0B8E8,
        0x4848A8,
        0x48A848,
        0xA84848 
    };
    memcpy((video_config*)&config0, &context0, sizeof(video_config));
    memcpy((mosaic_config*)settings0, &config0, sizeof(mosaic_config));
    settings0->codec = new rgb_palette_codec(config0);
    
    provider *p0 = new mosaic_provider(settings0);
    cipher_config *cfg = new cipher_config;
    cfg->encrypted_size = p0->payload_size();
    none_cipher *c0 = new none_cipher(cfg);
    frame_codec *codec0 = new frame_codec(p0, c0);
    cases["config0.json"] = codec0;

    return cases;
}

void test_parse(){
    video_config_parser *vcp = new video_config_parser;
    frame_codec_parser *fcp = set_up();
    const char *test_name = "frame_codec_parser_test.test_parse";
    printInfo(test_name);

    const path root = DATA_PARSER_PATH / "frame_codec_parser";
    filesystem::path test_path = root / "test_cases.json";
    fstream file(test_path, ios_base::in);
    stringstream buffer;
    buffer << file.rdbuf();
    string data = buffer.str();
    json array = json::parse(data);
    for(json object : array){
        fcp->context_in = vcp -> parse(object);
        frame_codec* codec = fcp -> parse(object["frameCodec"]);
        delete codec;
    }

    delete fcp;
    delete vcp;
    printPass(test_name);
}

void test_serialize(){
    const char *test_name = "frame_codec_parser_test.test_serialize";
    printInfo(test_name);

    auto cases = get_test_cases();
    frame_codec* codec = cases["config0.json"];
    frame_codec_parser *fcp = set_up();

    json object = fcp->serialize(codec);
    delete fcp;
    string object_str = object.dump(2);

    for(auto kvp : cases){
        delete kvp.second;
    }
    printInfo("%s", object_str.c_str());
    printPass(test_name);
}

int main(){
    test_parse();
    test_serialize();
    return 0;
}