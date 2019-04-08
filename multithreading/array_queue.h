#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* need two pointers to keep track of the head and tail*/
struct ArrayQueue
{
    int count;
    int capacity;
    int head;
    int tail;
    int queue[capacity];
    pthread_mutex_t lock;
};

struct ArrayQueue *createQueue(int capacity);

// void enqueue(struct ArrayQueue *queue, int data);

// struct int *dequeue(struct ArrayQueue *queue);

// int getSize(struct ArrayQueue *queue);