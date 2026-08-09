#include "data/korra_data.hpp"
#include "data_boxer/data/piecable_data.hpp"
#include "data_boxer/data/begin_label_data.hpp"
#include "data_boxer/data/pieced_data.hpp"
#include <cstdint>
#include <functional>
#include <vector>
#include <map>

using namespace std;
class data_unboxer {
private:
    vector<piecable_data*> _builiding_data;
    vector<korra_data*> _built_data;
    map<uint16_t, function<korra_data*()>> _data_builders;    

    int _select_by_id(int id);
    void _put_begin_label(begin_label_data *label);
    void _put_pieced(pieced_data *piece);
public:
    data_unboxer();
    void put_payload(char *payload, int payload_size);
    korra_data* unbox();
};