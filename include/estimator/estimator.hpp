#include <cstdint>
#include <vector>

using namespace std;
class estimator {
private:
    vector<void*> *_packages;
protected:
    int $payload_byte_count_total;
    uint16_t $package_size;
public:
    estimator();
    //Put IP package and estimate optimality of encoding. It returns estimation of optimality
    virtual float put_package(void*);
    vector<void*>* packages();
    virtual ~estimator();
};