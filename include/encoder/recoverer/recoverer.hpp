#pragma once
class recoverer {
public:
    virtual char *make_recoverable(char *data, int *data_length) = 0;
    virtual char *recover(char *data, int data_length) = 0;
};