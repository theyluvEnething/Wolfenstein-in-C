src = main.c renderer.c

all:
	gcc -o doom $(src) -lgdi32
clean:
	rm -f doom


test start: 
	run test/test.exe
test build:
	gcc -o test/test test/test.c