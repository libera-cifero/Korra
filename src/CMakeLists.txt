cmake_minimum_required(VERSION 4.3.1)
project(stegvideo)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BIN_DIR ../../bin)
set(PROVIDER_DIR encoder/provider)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BIN_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})
set(BASIC_BLOCK_DIR "encoder/provider/basic_block")

file(GLOB color_src "color.cpp")
file(GLOB math_src "math.c")
file(GLOB basic_block_config_src "encoder/provider/basic_block_config.c")
file(GLOB bbc_src "${BASIC_BLOCK_DIR}/basic_block_container.cpp")
file(GLOB bbp_src "${BASIC_BLOCK_DIR}/basic_block_pointer.cpp")
file(GLOB bbpp_src "${BASIC_BLOCK_DIR}/basic_block_pointer_proxy.cpp")
file(GLOB rect_src "${BASIC_BLOCK_DIR}/rect.c")
file(GLOB rgb_index_src "${BASIC_BLOCK_DIR}/rgb_index.c")

#color
add_library(color STATIC ${color_src})
target_include_directories(color PRIVATE ../include)

#math
add_library(math STATIC ${math_src})
target_include_directories(math PRIVATE ../include)

#rect
add_library(rect STATIC ${rect_src})
target_include_directories(rect PRIVATE ../include)

#rgb_index
add_library(rgb_index STATIC ${rgb_index_src})
target_include_directories(rgb_index PRIVATE ../include)

#basic_block
add_library(basic_block STATIC ${bbc_src} ${bbp_src} ${bbpp_src} ${basic_block_config_src})
target_link_libraries(basic_block PUBLIC color math rect rgb_index)
target_include_directories(basic_block PRIVATE ../include)