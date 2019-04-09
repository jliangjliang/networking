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
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
};

struct Queue *create_queue(int capacity);

void enqueue(struct Queue *queue, int data);

struct Node *dequeue(struct Queue *queue);

int get_size(struct Queue *queue);

int destory_queue(struct Queue *queue);