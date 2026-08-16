#include "parser_factory.hpp"
#include "video_socket.hpp"

class static_parser_factory : public parser_factory<video_socket*> {
public:
    json_parser<video_socket*>* build() override;
};