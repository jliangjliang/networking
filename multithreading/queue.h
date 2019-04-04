#include <stdio.h>
#include <stdlib.h>

struct Node
{
    struct Node *next;
    int data;
};

struct Queue
{
    struct Node *head, *tail, *temp, *headTemp;
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

}

struct Node *dequeue(struct Queue *queue)
{
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
}

void getSize(struct Queue *queue)
{
	printf("\nQueue Size: %d\n", queue->count);
}

int isEmpty(struct Queue *queue)
{
	if (queue->head == NULL && queue->tail == NULL)
		return 0;
	else
		return -1;
}

void display(struct Queue *queue)
{
	struct Node *headTemp = queue->head;

	if (queue->head == NULL && queue->tail ==NULL)
	{
		printf("Queue is empty\n");
		return;
	}

	while (headTemp != queue->tail)
	{
		printf("%d ", headTemp->data);
		headTemp = headTemp->next;
	}

	if (headTemp == queue->tail)
		printf("%d", headTemp->data);
}