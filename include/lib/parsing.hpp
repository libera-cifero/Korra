#include "json.hpp"

int parse_int(nlohmann::basic_json<> token);
uint32_t parse_uint32_t(nlohmann::basic_json<> token);
bool parse_bool(nlohmann::basic_json<> token);