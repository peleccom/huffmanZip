#ifndef SORTED_QUEUE_H
#define SORTED_QUEUE_H

typedef struct sorted_queue_t *sorted_queue_t;

sorted_queue_t sorted_queue_new(int (*compar)(const void *, const void *));
void sorted_queue_free(sorted_queue_t queue);

long sorted_queue_count(sorted_queue_t queue);

void *sorted_queue_pop(sorted_queue_t queue);
void *sorted_queue_peek(sorted_queue_t queue);

void sorted_queue_append(sorted_queue_t queue, void *x);

void *sorted_queue_next(sorted_queue_t queue, int is_start);
#endif
