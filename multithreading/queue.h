#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct Node
{
    struct Node *next;
    int data;
};

struct Queue
{
    struct Node *head, *tail, *temp;
    int count;
    pthread_mutex_t lock;
    pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
};

struct Node *createNode(int data)
{
	struct Node *newNode = (struct Node*)malloc(1*sizeof(struct Queue));
	newNode->data = data;
	newNode->next = NULL;
	return newNode;
}

struct Queue *createQueue()
{
	struct Queue *q = (struct Queue*)malloc(1*sizeof(struct Queue));
	pthread_mutex_init(&q->lock, NULL);
	q->head = q->tail = NULL;
	return q;
}

void enqueue(struct Queue *queue, int data)
{
	struct Node *newNode = createNode(data);

	pthread_mutex_lock(&queue->lock);
	if (queue->head == NULL)
	{
		queue->head = queue->tail = newNode;
	}
	else
	{
		queue->tail->next = newNode;
		queue->tail = newNode;
	}
	queue->count++;
	pthread_cond_signal(&queue->cond1);
	pthread_mutex_unlock(&queue->lock);

}

struct Node *dequeue(struct Queue *queue)
{
	struct Node *temp = queue->head;

	while(1)
	{
		pthread_mutex_lock(&queue->lock);
		if (queue->head == NULL)
		{
			pthread_cond_wait(&queue->cond1, &queue->lock);
		}

		if (queue->head->next != NULL)
		{
			queue->head = queue->head->next;
		}
		else
		{
			queue->head = NULL;
			queue->tail = NULL;
		}

		queue->count--;
		pthread_mutex_unlock(&queue->lock);
		return temp;
	}
}