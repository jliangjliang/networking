#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "array_queue_fork.h"

/* create a queue*/
struct array_queue *create_queue(int capacity)
{
	struct array_queue *q = (struct array_queue*)malloc(1*sizeof(struct array_queue));
	q->head = 0;
	q->tail = 0;
	q->count = 0;
	q->capacity = capacity;
	q->array = malloc(capacity*sizeof(int));
	pthread_mutex_init(&(q->lock), NULL);
	pthread_cond_init(&(q->not_empty), NULL);
	pthread_cond_init(&(q->not_full), NULL);
	return q;
}

/* enqueue a node if the queue is not full */
void enqueue(struct array_queue *queue, int data)
{
	// pthread_mutex_lock(&queue->lock);

	/* check if queue is full*/
	if (queue->count >= queue->capacity)
	{
		perror("queue is full");
		// pthread_cond_wait(&(queue->not_full), &(queue->lock));
	}

	/* enqueue */
	if (queue->count <= 0)
	{
		queue->array[0] = data;
		queue->head = 0;
		queue->tail = 0;
		queue->count = 1;
	}
	else if (queue->tail == queue->capacity - 1)
	{
		queue->array[0] = data;
		queue->tail = 0;
		queue->count++;
	}
	else
	{
		queue->array[(queue->tail)+1] = data;
		queue->tail++;
		queue->count++;	
	}

	printf("en queue count: %d\n", queue->count);

	// pthread_cond_signal(&queue->not_empty);
	// pthread_mutex_unlock(&queue->lock);
}

/* dequeue the head of the queue */
int dequeue(struct array_queue *queue)
{
	int head, data;

	// pthread_mutex_lock(&queue->lock);
	/* if the queue is empty, wait*/
	if (queue->count <= 0)
	{
		perror("queue is empty");
		// pthread_cond_wait(&(queue->not_empty), &(queue->lock));
	}

	/* dequeue */
	head = queue->head;
	data = queue->array[head];

	/* update */
	if (queue->head == queue->capacity - 1)
	{
		queue->head = 0;
		queue->count--;
	}
	else
	{
		queue->head++;
		queue->count--;
	}

	printf("de queue count: %d\n", queue->count);

	// pthread_cond_signal(&(queue->not_full));
	// pthread_mutex_unlock(&queue->lock);

	return data;
}

/* get the size of */
int get_size(struct array_queue *queue)
{
	// pthread_mutex_lock(&queue->lock);

	int size = queue->count;

	// pthread_mutex_unlock(&queue->lock);

	return size;
}

/* display elements in  queue */
void display_queue(struct array_queue *queue)
{
	int i;
	for (i = 0; i < queue->capacity; ++i)
	{
		/* code */
		printf("%d ", queue->array[i]);
	}
	printf("\n");
}

/* destroy queue */
void destroy_queue(struct array_queue *queue)
{
	free(queue->array);
	free(queue);
}