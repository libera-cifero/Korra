#include "config/data/video_config.hpp"
#include "video_pipe_in_manager.hpp"
class rutube_rtmp_in_manager : public video_pipe_in_manager {
public:
    rutube_rtmp_in_manager(video_config &config);
    video_pipe_in *build() override;
    void destroy(video_pipe_in *) override;
};