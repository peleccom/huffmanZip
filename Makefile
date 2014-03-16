all: main

main: hmzip.o
	gcc hmzip.o -o hmzip

hmzip.o: hmzip.c
	gcc -c src/hmzip.c

clean:
	rm -rf *.o hmzip
