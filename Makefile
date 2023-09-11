src = main.c renderer.c

all:
	gcc -o doom $(src) -lgdi32
test:
	gcc -E $(src) -lgdi32
clean:
	rm -f doom