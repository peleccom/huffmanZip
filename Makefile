.PHONY : clean all


CFLAGS = -g   --std=c99

all: main

main: hmzip.o huffman_tree.o sorted_queue.o archive_format.o
	gcc $(CFLAGS) hmzip.o  huffman_tree.o sorted_queue.o archive_format.o -o hmzip

hmzip.o: src/hmzip.c
	gcc $(CFLAGS) -c src/hmzip.c

huffman_tree.o: src/huffman_tree.c
	gcc $(CFLAGS) -c src/huffman_tree.c
	
sorted_queue.o: src/sorted_queue.c
	gcc $(CFLAGS) -c src/sorted_queue.c
	
archive_format.o: src/archive_format.c
	gcc $(CFLAGS) -c src/archive_format.c

clean:
	rm -rf *.o hmzip
