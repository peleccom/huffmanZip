all: testqueue

testqueue: sorted_queue.o test_sorted_queue.c
	gcc -g test_sorted_queue.c ../sorted_queue.o -o ../test

sorted_queue.o: ../src/sorted_queue.c
	gcc -c ../src/sorted_queue.c -o ../sorted_queue.o
