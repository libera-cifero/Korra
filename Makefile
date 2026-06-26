DEBUG_FLAG = -DCMAKE_BUILD_TYPE=Debug
RELEASE_FLAG = -DCMAKE_BUILD_TYPE=Release

arch:
	dot -Tsvg doc/architecture.dot > doc/map.svg

init_src:
	mkdir -p build
	mkdir -p build/src 
	@if [ -n "$${RELEASE+x}" ]; then \
		cmake -S src -B build/src ${RELEASE_FLAG}; \
	else \
		cmake -S src -B build/src ${DEBUG_FLAG}; \
	fi

init_test:
	mkdir -p build
	mkdir -p build/test
	@if [ -n "$${RELEASE+x}" ]; then \
		cmake -S test -B build/test ${RELEASE_FLAG}; \
	else \
		cmake -S test -B build/test ${DEBUG_FLAG}; \
	fi

init: init_src init_test

#SRC
#=====BEGIN=====
build_src:
	cmake --build build/src
color:
	cmake --build build/src --target color
basic_block: color
	cmake --build build/src --target basic_block
#======END======

#TESTS
#=====BEGIN=====
build_test: build_src init_test
	cmake --build build/test

color_test: color
	cmake --build build/test --target color_test && ./bin/color_test

basic_block_test: color basic_block
	cmake --build build/test --target basic_block_test && ./bin/basic_block_test
#======END======