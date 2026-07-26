class data {
public:
    virtual char *to_bytes(int bytes_size) = 0;
    virtual void from_bytes(char *bytes, int bytes_size);
};