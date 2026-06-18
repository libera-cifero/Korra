#include "encoder/provider/basic_provider.hpp"

basic_provider::basic_provider(basic_provider_config config){
    _config = config;
}

char* basic_provider::pull_data(char **frames, int *data_count) {
    
}

char** basic_provider::push_frames(char *data, int *frame_count) {

}

basic_provider::~basic_provider() {
    delete _config.reader;
}