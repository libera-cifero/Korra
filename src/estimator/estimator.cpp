#include "estimator/estimator.hpp"
#include <cstring>
#include <vector>

estimator::estimator(){
    _packages = new vector<void*>;
}

float estimator::put_package(void *ip_package) {
    _packages->push_back(ip_package);
    memcpy(&$package_size, (char*)ip_package + 2, 2);
    $payload_byte_count_total += $package_size;
    return 0;
}

vector<void*>* estimator::packages(){
    auto result = _packages;
    _packages = new vector<void*>;

    return result;
}

estimator::~estimator() {
    delete _packages; 
}