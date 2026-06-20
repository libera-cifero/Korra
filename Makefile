DEF_ARGS = -Iinclude -Itest/lib
DEF_TFILES = test/lib/test.c src/color.cpp #default files for test assembly
DEF_TARGS = ${DEF_ARGS} -g ${DEF_TFILES}

all: src/main.c src/color.cpp
	g++ ${DEF_ARGS} src/main.c src/color.cpp -o bin/stegvideo

color_test: ${DEF_TFILES} test/color_test.cpp
	g++ ${DEF_ARGS} test/color_test.cpp -o bin/color_test

bbc_test: ${DEF_TFILES} src/encoder/provider/basic_block_container.cpp test/encoder/provider/basic_block_container_test.cpp
	g++ ${DEF_TARGS} src/encoder/provider/basic_block_container.cpp test/encoder/provider/basic_block_container_test.cpp -o bin/bbc_test