#include "video_codec/frame_codec/cipher/cipher.hpp"
#include "lib/xchacha20.h"

class xchacha20_cipher : public cipher {
public:
    
    int header_size() override;
    char *encrypt(char *data) override;
    char *decrypt(char *encrypted) override;
};