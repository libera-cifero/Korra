#include <filesystem>
using namespace std::filesystem;
const path CWD = current_path();
const path CONTEXT_PATH = CWD / "test" / "context";
const path DATA_PATH = CONTEXT_PATH / "data";
const path EXPECTED_PATH = CONTEXT_PATH / "expected";