#include <filesystem>
using namespace std::filesystem;
const path CWD = current_path();
const path CONTEXT_PATH = CWD / "test" / "context";
const path DATA_PATH = CONTEXT_PATH / "data";
const path EXPECTED_PATH = CONTEXT_PATH / "expected";

const path EXPECTED_FRAME_PATH = EXPECTED_PATH / "frame";
const path DATA_FRAME_PATH = DATA_PATH / "frame";
const path DATA_COLOR_CODEC_PATH = DATA_PATH / "color_codec";