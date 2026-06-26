cmake_minimum_required(VERSION 4.3.1)
project(stegvideo)

set(BIN_DIR ../../bin)
set(PROVIDER_DIR encoder/provider)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BIN_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

file(GLOB color_src "color.cpp")
file(GLOB bbc_src "encoder/provider/basic_block/basic_block_container.cpp")
file(GLOB bbp_src "encoder/provider/basic_block/basic_block_pointer.cpp")
file(GLOB bbpp_src "encoder/provider/basic_block/basic_block_pointer_proxy.cpp")

message(STATUS "bbc_src = ${bbc_src}")
message(STATUS "bbp_src = ${bbp_src}")
message(STATUS "bbpp_src = ${bbpp_src}")

add_library(color STATIC ${color_src})

add_library(basic_block STATIC ${bbc_src} ${bbp_src} ${bbpp_src})
target_link_libraries(basic_block PRIVATE color)

target_include_directories(color PRIVATE ../include)
target_include_directories(basic_block PRIVATE ../include)