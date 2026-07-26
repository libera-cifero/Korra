#include <iterator>
using namespace std;

#define BYTE_ITERATOR \
template<random_access_iterator iterator_type> \
requires same_as<iter_value_t<iterator_type>, char>

BYTE_ITERATOR
class data_checker {
public:
    bool is_type_readable(iterator_type data_begin, iterator_type frame_end);
    virtual bool is_size_readable(iterator_type data_begin, iterator_type frame_end) = 0;
    virtual bool is_data_readable(iterator_type data_begin, iterator_type frame_end) = 0;

    virtual ~data_checker();
};