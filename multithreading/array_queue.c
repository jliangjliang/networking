#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "array_queue.h"

/* create a queue*/
struct ArrayQueue *createQueue(int capacity)
{
	struct Queue *q = (struct Queue*)malloc(1*sizeof(struct Queue));
	pthread_mutex_init(&q->lock, NULL);
	q->head = q->tail = NULL;
	q->capacity = capacity;
	return q;
}

// /* enqueue a node if the queue is not full */
// void enqueue(struct Queue *queue, int data)
// {
// 	struct Node *newNode = createNode(data);

// 	pthread_mutex_lock(&queue->lock);

// 	/* check if queue is full*/
// 	if (queue->count < queue->capacity)
// 	{
// 		/* enqueue */
// 		if (queue->head == NULL)
// 		{
// 			queue->head = newNode;
// 			queue->tail = newNode;
// 		}
// 		else
// 		{
// 			queue->tail->next = newNode;
// 			queue->tail = newNode;
// 		}

// 		/* update count*/
// 		queue->count++;
// 		// pthread_cond_signal(&queue->notEmpty);
// 	}
// 	else
// 	{
// 		perror("queue is full");
// 		return;
// 	}

// 	pthread_mutex_unlock(&queue->lock);

// }

// /* dequeue the head of the queue */
// struct Node *dequeue(struct Queue *queue)
// {
// 	struct Node *temp = queue->head;

// 	while(1)
// 	{
// 		pthread_mutex_lock(&queue->lock);
// 		/* if the queue is empty, continue*/
// 		if (queue->head == NULL)
// 		{
// 			continue;
// 			// pthread_cond_wait(&queue->notEmpty, &queue->lock);
// 		}

// 		/* dequeue */
// 		if (queue->head->next != NULL)
// 		{
// 			queue->head = queue->head->next;
// 		}
// 		else
// 		{
// 			queue->head = NULL;
// 			queue->tail = NULL;
// 		}

// 		/* update count*/
// 		queue->count--;
// 		pthread_mutex_unlock(&queue->lock);
// 		return temp;
// 	}
// }

// /* get the size of */
// int getSize(struct Queue *queue)
// {
// 	pthread_mutex_lock(&queue->lock);

// 	int size = queue->count;

// 	pthread_mutex_unlock(&queue->lock);

// 	return size;
// }