#pragma once
#include "data_boxer/data/base/identifiable.hpp"
#include "data_boxer/data/korra_data.hpp"

class piecable_data : public korra_data, public identifiable { 
private:
    int _writen;
    bool _is_built = false, _is_inited = false;
protected:
    int __buffer_size;
    char *__buffer;//__buffer is not deleting, because __buffer is processing by out agent
public:
    void set_id(int id);
    void init_buffer(int buffer_size);
    virtual int size() override;
    bool is_built();
    void put_piece(char *piece, int piece_size);
};