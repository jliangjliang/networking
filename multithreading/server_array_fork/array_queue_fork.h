#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* need two pointers to keep track of the head and tail*/
struct array_queue
{
	int head;
    int tail;
    int count;
    int capacity;
    int *array;
    pthread_mutex_t lock;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
};

/* create queue */
struct array_queue *create_queue(int capacity);

/* enqueue */
void enqueue(struct array_queue *queue, int data);

/* dequeue */
int dequeue(struct array_queue *queue);

/* get size of a queue*/
int get_size(struct array_queue *queue);

/* display elements in queue */
void display_queue(struct array_queue *queue);

/* destroy a queue*/
void destroy_queue(struct array_queue *queue);