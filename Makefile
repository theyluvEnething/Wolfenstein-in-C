src = src/main.c src/renderer.c src/func.c src/level.c src/vector2.c src/input.c
bin = -lgdi32 -lgmp


all:
	gcc -o doom $(src) -lgdi32 -I. -L./lib -lgmp -lm
debug:
	gcc -g -o doom $(src) -lgdi32 -I. -L./lib -lgmp -lm
run: debug
	doom.exe

test start: 
	run test/test.exe
test build:
	gcc -o test/test test/test.c