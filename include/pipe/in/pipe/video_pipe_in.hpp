#pragma once

class video_pipe_in {
public:
    virtual char *read(int size) = 0;
    virtual ~video_pipe_in(){};
};