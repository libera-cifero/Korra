#include "estimator.hpp"

class none_estimator : public estimator {
public:
    float put_package(void*) override { return 100; };
};