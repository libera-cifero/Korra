#include "parser_factory.hpp"
#include "video/video_socket.hpp"

class static_parser_factory : public parser_factory {
public:
    json_parser<video_listener*>* build() override;
};