typedef struct video_encoder_config{
    int window_width;
    int window_height;
    int pixel_size;
    int color_resolution;
    int fps;
} video_encoder_config;

struct encode_result{
    char *image_data;
    
};

class video_encoder {
private:
    video_encoder_config _config;
public:
    video_encoder(video_encoder_config config);
};