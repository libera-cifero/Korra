init_src:
	mkdir -p build
	mkdir -p build/src
	cmake -S src -B build/src

init_test:
	mkdir -p build
	mkdir -p build/test
	cmake -S test -B build/test

build_src:
	cmake --build build/src

build_test: init_test
	cmake --build build/test

color_test: init_test
	cmake --build build/test --target color_test && ./bin/color_test

bbc_test: init_test
	cmake --build build/test --target bbc_test && ./bin/bbc_test

#rilr_test: