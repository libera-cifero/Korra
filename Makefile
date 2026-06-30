#COMMON
#=====BEGIN=====
init_release: init_src_release init_test_release
init_debug: init_src_debug init_test_debug

arch:
	dot -Tsvg doc/architecture.dot > doc/map.svg
#======END======

#SRC
#=====BEGIN=====
init_src_debug:
	python3 tool/cmake_init.py src Debug
init_src_release:
	python3 tool/cmake_init.py src Release
build_src:
	cmake --build build/src

color:
	cmake --build build/src --target color
math:
	cmake --build build/src --target math
basic_block: color math
	cmake --build build/src --target basic_block
#======END======

#TESTS
#=====BEGIN=====
init_test_debug:
	python3 tool/cmake_init.py test Debug
init_test_release:
	python3 tool/cmake_init.py test Release
build_test: build_src
	cmake --build build/test

test: build_test
	python3 tool/test.py
color_test: color
	cmake --build build/test --target color_test && python3 tool/run.py color_test
basic_block_test: color basic_block
	cmake --build build/test --target basic_block_test && python3 tool/run.py basic_block_test
math_test: math
	cmake --build build/test --target math_test && python3 tool/run.py math_test
#======END======

#TEST TOOLS
#=====BEGIN=====

#======END======