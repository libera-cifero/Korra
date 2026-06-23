cmake_minimum_required(VERSION 4.3.1)

project(stegvideo.test)

set(BIN_DIR ../../bin)
set(PROVIDER_DIR encoder/provider)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BIN_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

