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
    sem_t mutex;
};

/* create queue */
void *ini_queue(struct array_queue *queue, int capacity);

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