all: geometry

geometry: geometry.c
	gcc -Wall -Werror -o geometry geometry.c

clean:
	rm geometry

run:
	./geometry