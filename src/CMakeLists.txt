cmake_minimum_required(VERSION 4.3.1)
project(stegvideo)

set(BIN_DIR ../../bin)
set(PROVIDER_DIR encoder/provider)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BIN_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

file(GLOB color_src "color.cpp")
file(GLOB bbc_src "encoder/provider/basic_block_container.cpp")

add_library(color STATIC ${color_src})

add_library(bbc STATIC ${bbc_src})
target_link_libraries(bbc ${color})

target_include_directories(color PRIVATE ../include)
target_include_directories(bbc PRIVATE ../include)