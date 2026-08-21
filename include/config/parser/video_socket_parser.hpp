#include "config/parser/frame_codec/frame_codec_parser.hpp"
#include "parser.hpp"
#include "video_socket.hpp"

class video_socket_parser : public json_parser<video_socket*>
{
private:
    frame_codec_parser *_frame_codec_parser = nullptr;
    
public:
    video_socket* parse(json) override;
    json serialize(video_socket*) override;
    ~video_socket_parser() override;
};