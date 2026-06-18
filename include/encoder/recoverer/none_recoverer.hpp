#include "recoverer.hpp"

class none_recoverer : public recoverer {
public:
    char *make_recoverable(char *data, int *data_length) override;
    char *recover(char *data, int data_length) override;
};