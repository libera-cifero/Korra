#include "pipe/video_pipe_in.hpp"
class video_pipe_in_manager {
public:
    virtual video_pipe_in *build() = 0;
    virtual void destroy(video_pipe_in *) = 0;

    virtual ~video_pipe_in_manager() = default;
};