#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "queue.h"

/* create a node */
struct Node *create_node(int data)
{
	struct Node *newNode = (struct Node*)malloc(1*sizeof(struct Queue));
	newNode->data = data;
	newNode->next = NULL;
	return newNode;
}


/* create a queue*/
struct Queue *create_queue(int capacity)
{
	struct Queue *q = (struct Queue*)malloc(1*sizeof(struct Queue));
	pthread_mutex_init(&(q->lock), NULL);
	pthread_cond_init(&(q->not_empty), NULL);
	pthread_cond_init(&(q->not_full), NULL);
	q->head = NULL;
	q->tail = NULL;
	q->capacity = capacity;
	return q;
}

/* enqueue a node if the queue is not full */
void enqueue(struct Queue *queue, int data)
{
	struct Node *newNode = create_node(data);

	pthread_mutex_lock(&queue->lock);

	/* check if queue is full*/
	if (queue->count < queue->capacity)
	{
		/* enqueue */
		if (queue->head == NULL)
		{
			queue->head = newNode;
			queue->tail = newNode;
		}
		else
		{
			queue->tail->next = newNode;
			queue->tail = newNode;
		}

		/* update count*/
		queue->count++;
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
struct Node *dequeue(struct Queue *queue)
{
	struct Node *temp = queue->head;

	// while(1)
	// {
		pthread_mutex_lock(&queue->lock);
		/* if the queue is empty, continue*/
		if (queue->head == NULL)
		{
			// continue;
			pthread_cond_wait(&queue->not_empty, &queue->lock);
		}

		/* dequeue */
		if (queue->head->next != NULL)
		{
			queue->head = queue->head->next;
		}
		else
		{
			queue->head = NULL;
			queue->tail = NULL;
		}

		/* update count*/
		queue->count--;
		pthread_mutex_unlock(&queue->lock);
		return temp;
	// }
}

/* get the size of */
int get_size(struct Queue *queue)
{
	pthread_mutex_lock(&queue->lock);

	int size = queue->count;

	pthread_mutex_unlock(&queue->lock);

	return size;
}