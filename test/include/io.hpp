#include <filesystem>
#include <string>
using namespace std::filesystem;
const path CWD = current_path();
const path CONTEXT_PATH = CWD / "test" / "context";
const path DATA_PATH = CONTEXT_PATH / "data";
const path EXPECTED_PATH = CONTEXT_PATH / "expected";

const path EXPECTED_FRAME_PATH = EXPECTED_PATH / "frame";
const path DATA_FRAME_PATH = DATA_PATH / "frame";
const path DATA_FRAME_CODEC_PATH = DATA_PATH / "frame_codec";

const path DATA_OUT_PATH = DATA_PATH / "out";

const path DATA_PARSER_PATH = DATA_PATH / "config" / "parser";

path to_native_path(std::string file_name);