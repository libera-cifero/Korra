//TEST IT MANUALLY!!!!!
#include "config/data/video_config.hpp"
#include "config/parser/parser.hpp"
#include "config/parser/video_config_parser.hpp"
#include "test.hpp"
#include "io.hpp"
#include <filesystem>
#include <ios>
#include <vector>
#include <fstream>

void test_parse(){
    //DEBUG IT WHEN YOU TEST!!!
    const char *test_name = "video_config_parser_test.test_parse";
    printInfo(test_name);

    const path root = DATA_PARSER_PATH / "video_config_parser";
    vector<filesystem::path> files = { root / "config0.json", root / "config1.json" };
    for(path p : files){
        fstream file(p, ios_base::in);
        stringstream buffer;
        buffer << file.rdbuf();
        string data = buffer.str();
        json object = json::parse(data);
        video_config_parser parser;
        video_config conf = parser.parse(object);
        printInfo(data.c_str());
        printInfo("frame_width=%d, frame_height=%d, fps=%d", conf.frame_width, conf.frame_height, conf.fps);
    }

    printPass(test_name);
}

void test_serialize(){
    //DEBUG IT WHEN YOU TEST!!!
    const char *test_name = "video_config_parser_test.test_serialize";
    printInfo(test_name);

    vector<video_config> configs = {
        { .frame_width = 2, .frame_height = 200, .fps = 40 },
        { .frame_width = 2111, .frame_height = 1741, .fps = 100 }
    };

    video_config_parser p;
    for(video_config c : configs) {
        json data = p.serialize(c);
        printInfo("frame_width=%d, frame_height=%d, fps=%d", c.frame_width, c.frame_height, c.fps);
        printInfo(to_string(data).c_str());
    }

    printPass(test_name);
}

int main(){
    test_parse();
    test_serialize();
    return 0;
}