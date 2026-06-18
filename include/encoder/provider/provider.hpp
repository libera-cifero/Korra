#pragma once
class provider {
public:
    virtual char *pull_data_from_frames(char **frames, int frame_count) = 0;
    virtual char **push_frames(char *data, int *frame_count) = 0;
};