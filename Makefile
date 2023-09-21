src = src/main.c src/renderer.c src/func.c src/level.c

all:
	gcc -o doom $(src) -lgdi32 -lm
debug:
	gcc -g -o doom $(src) -lgdi32 -lm
clean:
	rm -f doom

test start: 
	run test/test.exe
test build:
	gcc -o test/test test/test.c