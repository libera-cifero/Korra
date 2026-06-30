cmake_minimum_required(VERSION 4.3.1)
project(stegvideo)

set(BIN_DIR ../../bin)
set(PROVIDER_DIR encoder/provider)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BIN_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

file(GLOB color_src "color.cpp")
file(GLOB math_src "math.c")
file(GLOB basic_block_config_src "encoder/provider/basic_block_config.c")
file(GLOB bbc_src "encoder/provider/basic_block/basic_block_container.cpp")
file(GLOB bbp_src "encoder/provider/basic_block/basic_block_pointer.cpp")
file(GLOB bbpp_src "encoder/provider/basic_block/basic_block_pointer_proxy.cpp")

#color
add_library(color STATIC ${color_src})
target_include_directories(color PRIVATE ../include)

#math
add_library(math STATIC ${math_src})
target_include_directories(math PRIVATE ../include)

#basic_block
add_library(basic_block STATIC ${bbc_src} ${bbp_src} ${bbpp_src} ${basic_block_config_src} ${math_src})
target_link_libraries(basic_block PRIVATE color)
target_link_libraries(basic_block PRIVATE math)
target_include_directories(basic_block PRIVATE ../include)