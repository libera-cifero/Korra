cmake_minimum_required(VERSION 4.3.1)

project(stegvideo.test)

set(BIN_DIR ../../bin)
set(INCLUDE_DIR "lib" "../include")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

file(GLOB test_lib_src "src/lib/test.c")
file(GLOB color_test_src "src/color_test.cpp")
file(GLOB basic_block_test_src "src/encoder/provider/basic_block_test.cpp")
file(GLOB math_test_src "src/math_test.cpp")

macro(target_setup target)
    target_link_directories(${target} PUBLIC ../bin/lib)
    target_include_directories(${target} PUBLIC ${INCLUDE_DIR})
endmacro()

#color_test
add_executable(color_test ${test_lib_src} ${color_test_src})
target_setup(color_test)
target_link_libraries(color_test PRIVATE color)

#basic_block_test
add_executable(basic_block_test ${test_lib_src} ${basic_block_test_src})
target_setup(basic_block_test)
target_link_libraries(basic_block_test PRIVATE color basic_block)

#math_test
add_executable(math_test ${test_lib_src} ${math_test_src})
target_setup(math_test)
target_link_libraries(math_test PRIVATE math)