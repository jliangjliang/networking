#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "array_queue.h"

/* create a queue*/
struct ArrayQueue *createQueue(int capacity)
{
	struct ArrayQueue *q = (struct ArrayQueue*)malloc(1*sizeof(struct ArrayQueue));
	q->head = -1;
	q->tail = -1;
	q->count = -1;
	q->capacity = capacity;
	q->array = malloc(capacity*sizeof(int));
	pthread_mutex_init(&q->lock, NULL);
	return q;
}

/* enqueue a node if the queue is not full */
void enqueue(struct ArrayQueue *queue, int data)
{
	pthread_mutex_lock(&queue->lock);
	printf("enqueue data is: %d\n", data);

	/* check if queue is full*/
	if (queue->count < queue->capacity)
	{
		/* enqueue */
		if (queue->count < 0)
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
		// pthread_cond_signal(&queue->notEmpty);
	}
	else
	{
		perror("queue is full");
	}

	pthread_mutex_unlock(&queue->lock);
	return;

}

/* dequeue the head of the queue */
int dequeue(struct ArrayQueue *queue)
{
	int head = queue->head;
	int data = queue->array[head];
	printf("dequeue data is: %d\n", data);

	while(1)
	{
		pthread_mutex_lock(&queue->lock);
		/* if the queue is empty, continue*/
		if (queue->count < 0)
		{
			pthread_mutex_unlock(&queue->lock);
			continue;
			// pthread_cond_wait(&queue->notEmpty, &queue->lock);
		}

		queue->head++;
		queue->count--;

		pthread_mutex_unlock(&queue->lock);
		return data;
	}
}

/* get the size of */
int getSize(struct ArrayQueue *queue)
{
	pthread_mutex_lock(&queue->lock);

	int size = queue->count;

	pthread_mutex_unlock(&queue->lock);

	return size;
}

void displayQueue(struct ArrayQueue *queue)
{
	int i;
	for (i = 0; i < queue->capacity; ++i)
	{
		/* code */
		printf("%d ", queue->array[i]);
	}
	printf("\n");
}