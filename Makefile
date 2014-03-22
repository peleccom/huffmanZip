all: main

main: hmzip.o huffman_tree.o sorted_queue.o
	gcc --std=c99 hmzip.o  huffman_tree.o sorted_queue.o -o hmzip

hmzip.o: src/hmzip.c
	gcc --std=c99 -c src/hmzip.c

huffman_tree.o: src/huffman_tree.c
	gcc --std=c99 -c src/huffman_tree.c
	
sorted_queue.o: src/sorted_queue.c
	gcc --std=c99 -c src/sorted_queue.c

clean:
	rm -rf *.o hmzip
