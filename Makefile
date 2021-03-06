.PHONY : clean all


CFLAGS = -g -Wall --std=c99
BUILDDIR=out
TEST_FILENAME=testfile

all: main

test: main
	/usr/bin/time -p sh -c "./hmzip -c $(TEST_FILENAME) -o $(TEST_FILENAME).hz;./hmzip -x $(TEST_FILENAME).hz -o $(TEST_FILENAME).extracted"
	diff $(TEST_FILENAME)  $(TEST_FILENAME).extracted

main:  $(BUILDDIR)/hmzip.o  $(BUILDDIR)/huffman_tree.o  $(BUILDDIR)/sorted_queue.o  $(BUILDDIR)/archive_format.o
	gcc $(CFLAGS)  $(BUILDDIR)/hmzip.o   $(BUILDDIR)/huffman_tree.o  $(BUILDDIR)/sorted_queue.o  $(BUILDDIR)/archive_format.o -o hmzip

 $(BUILDDIR)/hmzip.o: src/hmzip.c
	mkdir -p $(BUILDDIR)
	gcc $(CFLAGS) -c src/hmzip.c -o $(BUILDDIR)/hmzip.o

 $(BUILDDIR)/huffman_tree.o: src/huffman_tree.c
	mkdir -p $(BUILDDIR)
	gcc $(CFLAGS) -c src/huffman_tree.c  -o $(BUILDDIR)/huffman_tree.o
	
 $(BUILDDIR)/sorted_queue.o: src/sorted_queue.c
	mkdir -p $(BUILDDIR)
	gcc $(CFLAGS) -c src/sorted_queue.c  -o $(BUILDDIR)/sorted_queue.o
	
 $(BUILDDIR)/archive_format.o: src/archive_format.c
	mkdir -p $(BUILDDIR)
	gcc $(CFLAGS) -c src/archive_format.c  -o $(BUILDDIR)/archive_format.o

clean:
	rm -rf   $(BUILDDIR)/*.o hmzip
