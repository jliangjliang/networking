#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* node structure */
struct Node
{
    struct Node *next;
    int data;
};

/* queue structure */
struct Queue
{
    struct Node *head, *tail, *temp;
    int count;
    int capacity;
    pthread_mutex_t lock;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
};

/* create queue */
struct Queue *create_queue(int capacity);

/* enqueue */
void enqueue(struct Queue *queue, int data);

/* dequeue */
struct Node *dequeue(struct Queue *queue);

/* get size of a queue*/
int get_size(struct Queue *queue);

/* destroy a queue*/
int destory_queue(struct Queue *queue);