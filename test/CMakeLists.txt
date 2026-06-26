cmake_minimum_required(VERSION 4.3.1)

project(stegvideo.test)

set(BIN_DIR ../../bin)
set(INCLUDE_DIR "lib" "../include")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

file(GLOB test_lib_src "lib/test.c")
file(GLOB color_test_src "color_test.cpp")
file(GLOB basic_block_test_src "encoder/provider/basic_block_test.cpp")

#color_test
add_executable(color_test ${test_lib_src} ${color_test_src})
target_link_directories(color_test PUBLIC ../bin/lib)
target_link_libraries(color_test color)
target_include_directories(color_test PUBLIC ${INCLUDE_DIR})

#basic_block_test
add_executable(basic_block_test ${test_lib_src} ${basic_block_test_src})
target_link_directories(basic_block_test PUBLIC ../bin/lib)
target_link_libraries(basic_block_test PRIVATE color basic_block)
target_include_directories(basic_block_test PUBLIC ${INCLUDE_DIR})