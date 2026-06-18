ARGS=1

all: src/main.c
	g++ src/main.c src/color.cpp -o bin/stegvideo

color_test: src/color.cpp test/color_test.cpp test/test.c 
	g++ src/color.cpp test/color_test.cpp test/test.c -g -o bin/color_test