#include "config/data/video_config.hpp"
#include "video_pipe_out_manager.hpp"

class rutube_rtmp_out_manager : public video_pipe_out_manager {
private:
    video_config _config;
public:
    rutube_rtmp_out_manager(video_config &config);
    video_pipe_out *build() override;
    void destroy(video_pipe_out *) override;
};