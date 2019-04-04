#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

struct Node
{
    struct Node *next;
    int data;
};

struct Queue
{
    struct Node *head, *tail, *temp;
    int count;
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
	q->head = q->tail = NULL;
	return q;
}

void enqueue(struct Queue *queue, int data)
{
	struct Node *newNode = createNode(data);

	// pthread_mutex_lock(&lock);
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
	// pthread_mutex_unlock(&lock);

}

struct Node *dequeue(struct Queue *queue)
{
	// pthread_mutex_lock(&lock);
	if (queue->head == NULL)
	{
		perror("Error: dequeue from an empty queue.");
		return NULL;
	}
	else
	{
		struct Node *temp = queue->head;
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
		return temp;
	}
	// pthread_mutex_unlock(&lock);
}