all: src/main.c
	gcc src/main.c src/color.c -o bin/stegvideo

color_test: src/color.c test/color_test.c 
	gcc src/color.c test/color_test.c -o bin/color_test
	./bin/color_test
	rm -f bin/color_test