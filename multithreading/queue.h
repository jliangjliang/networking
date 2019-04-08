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
    int capacity;
    pthread_mutex_t lock;
    // pthread_cond_t notEmpty = PTHREAD_COND_INITIALIZER;
    // pthread_cond_t notFull = PTHREAD_COND_INITIALIZER;
};

struct Queue *createQueue(int capacity);

void enqueue(struct Queue *queue, int data);

struct Node *dequeue(struct Queue *queue);

int getSize(struct Queue *queue);