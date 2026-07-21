#include "corrector.hpp"

class none_corrector : public corrector {
public:
    char *make_recoverable(char *data, int *data_length) override;
    char *recover(char *data, int data_length) override;
};