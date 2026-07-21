#pragma once
#include "config/data/pipes_config.hpp"
#include "config/parser/parser.hpp"
#include "video_pipe_in_parser.hpp"
#include "video_pipe_out_parser.hpp"
#include "video/pipe/video_pipe_in.hpp"
#include "video/pipe/video_pipe_out.hpp"
#include <map>
#include <string>
#include <tuple>
#include <vector>

using namespace std;
class pipes_parser : public json_parser<pipes_config>{
private:
    map<string, tuple<video_pipe_in_parser*, video_pipe_out_parser*>> _parser_map;
    map<string, video_pipe_in_parser*> _parser_in_map;
    map<string, video_pipe_out_parser*> _parser_out_map; 

    vector<function<video_pipe_in_parser*(video_pipe_in*)>> _parser_getters_in;
    vector<function<video_pipe_out_parser*(video_pipe_out*)>> _parser_getters_out;

    template<typename T>
    function<video_pipe_in_parser*(video_pipe_in*)> _make_in_get_if_type(string key);

    template<typename T>
    function<video_pipe_out_parser*(video_pipe_out*)> _make_out_get_if_type(string key);


    string _get_available_types_string();

    video_pipe_in_parser *_get_in_parser_by_pipe(video_pipe_in *pipe);
    video_pipe_out_parser *_get_out_parser_by_pipe(video_pipe_out *pipe);
public:
    video_pipe_in_parser *in;
    video_pipe_out_parser *out;

    pipes_parser();

    pipes_config parse(json) override;
    json serialize(pipes_config) override;
    ~pipes_parser();
};