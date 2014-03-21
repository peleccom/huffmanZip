#include <stdio.h>
#include <stdlib.h>
#include "../src/sorted_queue.h"


int cmp(const void *a, const void *b){
	int a_val = *((int *)a);
	int b_val = *((int *)b);
	if (a_val>b_val) return 1;
	else
		if (a_val<b_val) return -1;
	
	return 0;
}

int main(){
	int a = 5;
	int b = 6;
	int c = 7;
	int d = 1;
	int e = 10;
	sorted_queue_t queue;
	queue = sorted_queue_new(cmp);
	printf("elements in queue: %li\n", sorted_queue_count(queue));
	sorted_queue_append(queue, &b);
	sorted_queue_append(queue, &a);
	sorted_queue_append(queue, &e);
	sorted_queue_append(queue, &d);
	sorted_queue_append(queue, &c);
	printf("////\n");
	while(sorted_queue_count(queue))
	{
		printf("elements in queue: %li\n", sorted_queue_count(queue));
		printf("poped value is %i\n", *((int*)(sorted_queue_pop(queue))));
	}
	printf("elements in queue: %li\n", sorted_queue_count(queue));
	sorted_queue_append(queue, &a);
	printf("peeked value is %i\n", *((int*)(sorted_queue_peek(queue))));
	printf("elements in queue: %li\n", sorted_queue_count(queue));
	sorted_queue_free(queue);
	return 0;
}
