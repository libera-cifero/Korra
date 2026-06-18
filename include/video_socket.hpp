//Reads and writes frames for one IP package
class video_socket {
public:
    char **read_frames(int *frames_readed);
    void write_frames(char **frames, int frames_to_write);
};