#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "array_queue.h"

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
	// pthread_cond_init(&q->full, NULL);
	pthread_cond_init(&q->not_empty, NULL);
	return q;
}

/* enqueue a node if the queue is not full */
void enqueue(struct array_queue *queue, int data)
{
	pthread_mutex_lock(&queue->lock);
	printf("enqueue data is: %d\n", data);

	/* check if queue is not full*/
	if (queue->count < queue->capacity)
	{
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
			queue->array[queue->tail+1] = data;
			queue->tail++;
			queue->count++;
		}
		pthread_cond_signal(&queue->not_empty);
	}
	else
	{
		perror("queue is full");
		pthread_mutex_unlock(&queue->lock);
		return;
	}

	pthread_mutex_unlock(&queue->lock);
}

/* dequeue the head of the queue */
int dequeue(struct array_queue *queue)
{
	int head = queue->head;
	int data = queue->array[head];
	printf("dequeue data is: %d\n", data);

	// while(1)
	// {
		pthread_mutex_lock(&queue->lock);
		/* if the queue is empty, continue*/
		if (queue->count <= 0)
			pthread_cond_wait(&queue->not_empty, &queue->lock);
		
		queue->head++;
		queue->count--;

		pthread_mutex_unlock(&queue->lock);
		return data;
	// }
}

/* get the size of */
int get_size(struct array_queue *queue)
{
	pthread_mutex_lock(&queue->lock);

	int size = queue->count;

	pthread_mutex_unlock(&queue->lock);

	return size;
}

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