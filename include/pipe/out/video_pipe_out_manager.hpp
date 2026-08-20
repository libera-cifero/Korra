#include "pipe/video_pipe_out.hpp"

class video_pipe_out_manager {
public:
    virtual video_pipe_out *build() = 0;
    virtual void destroy(video_pipe_out *) = 0;
};