#pragma once
class provider {
public:
    virtual char *pull_data(char **frames, int *data_count) = 0;
    virtual char **push_frames(char *data, int *frame_count) = 0;
    virtual ~provider() { };
};