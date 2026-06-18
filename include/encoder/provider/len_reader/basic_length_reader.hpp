#include "data_length_reader.hpp"

class basic_length_reader 
{
public:
    int read(const char *first_frame, int frame_size);
    ~basic_length_reader();
};