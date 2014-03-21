#include <stdlib.h>

#include "sorted_queue.h"
#include "my_utils.h"

typedef int (*t_cmp_func)(const void *, const void *);


struct sorted_queue_element{
	struct sorted_queue_element *next;
	void *data;
};

struct sorted_queue_t{
	struct sorted_queue_element *head;
	long count;
	t_cmp_func cmp_f;
};


long sorted_queue_count(sorted_queue_t queue){
	return queue->count;
}

sorted_queue_t sorted_queue_new(int (*compar)(const void *, const void *)){
	sorted_queue_t new_sorted_queue;
	new_sorted_queue = malloc(sizeof(struct sorted_queue_t));
	CHKPTR(new_sorted_queue);
	new_sorted_queue->count = 0;
	new_sorted_queue->cmp_f = compar;
	new_sorted_queue->head = NULL;
	return new_sorted_queue;
}

void sorted_queue_append(sorted_queue_t queue, void *x){
	struct sorted_queue_element *element, **prev_element;
	struct sorted_queue_element *new_element = malloc(sizeof(
			struct sorted_queue_element));
	CHKPTR(new_element);
	queue->count ++;
	element = queue->head;
	prev_element = &queue->head;
	do
	{
		if (element == NULL)
		{
			*prev_element = new_element;
			new_element->data = x;
			new_element->next = NULL;
			return;
		}
		if (queue->cmp_f(x, element->data) <= 0)
		{
			// insert in current position
			new_element->data = element->data; // save data
			element->data = x;
			new_element->next = element->next;
			element->next = new_element;
			return;
		}
		prev_element = &element->next;
		element = element->next;
	}
	while(1);
}


void *sorted_queue_peek(sorted_queue_t queue){
	struct sorted_queue_element *element;
	element = queue->head;
	if (element){
		return element->data;
	}
	else
		return NULL;
}

void *sorted_queue_pop(sorted_queue_t queue){
	struct sorted_queue_element *element;
	void *data;
	element = queue->head;
	if (element){
		queue->head = element->next;
		queue->count --;
		data = element->data;
		free(element);
		return data;
	}
	else
		return NULL;
}

void sorted_queue_free(sorted_queue_t queue){
	// user creates new element and malloc memory for it, so i don't have 
	// any rights to free memory for elements, so i just free elemetns structures
	
	while(queue->head != NULL)
	{
		sorted_queue_pop(queue);
	};
	free(queue);
}
