#include "../parser/parser.hpp"
#include "video/video_socket.hpp"

class parser_factory {
public:
    virtual json_parser<video_listener*>* build() = 0;
};