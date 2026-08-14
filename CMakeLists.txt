cmake_minimum_required(VERSION 4.3.1)
project(korra)

find_path(CRYPTOPP_INCLUDE cryptopp/cryptlib.h /usr/include /usr/local/include /opt/homebrew/include)
find_library(CRYPTOPP_LIB cryptopp /usr/lib /usr/local/lib /opt/homebrew/lib)
if(NOT CRYPTOPP_INCLUDE OR NOT CRYPTOPP_LIB)
    message(FATAL_ERROR "Crypto++ is not found!")
endif()

set(BIN_DIR ${CMAKE_SOURCE_DIR}/bin)
set(LIB_DIR ${BIN_DIR}/lib)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include ${CRYPTOPP_INCLUDE})

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIB_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

add_subdirectory(src)
add_subdirectory(test)