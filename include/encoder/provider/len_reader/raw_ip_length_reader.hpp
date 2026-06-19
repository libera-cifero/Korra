#include "data_length_reader.hpp"

class raw_ip_length_reader : public data_length_reader
{
public:
    int read(const char *first_frame, int frame_size) override;
    ~raw_ip_length_reader() override;
};