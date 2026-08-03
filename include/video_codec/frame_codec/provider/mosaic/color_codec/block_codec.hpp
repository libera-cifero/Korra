class block_codec {
private:
    int _bits_per_number, _block_size;
    char *_frame;
public:
    block_codec(int bits_per_number, int block_size, char *frame);
    int bits_per_number();
    virtual void encode(int number, int block_index) = 0;
    virtual int decode(int block_index) = 0;
};