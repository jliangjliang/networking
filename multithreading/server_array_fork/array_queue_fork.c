#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <syscall.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/file.h>
#include <semaphore.h>
#include <sys/file.h>
#include "array_queue_fork.h"

/* create a queue*/
void *ini_queue(struct array_queue *q, int capacity)
{
	q->head = 0;
	q->tail = 0;
	q->count = 0;
	q->capacity = capacity;
	q->array = ptr;
	sem_init(&q->mutex, 1, 1);
}

/* need to create a shared memnory for the array? */
void *shared_array(int capacity)
{
	int *array = (int *) malloc(capacity*sizeof(int));
	const char *memname = "array";
	const size_t region_size = sysconf(_SC_PAGE_SIZE);
	int fd = shm_open(memname, O_CREAT | O_TRUNC | O_RDWR, 0666);
	int r = ftruncate(fd, region_size);
	write(fd, &array, sizeof(int)); // size of int?
	int *ptr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	close(fd);

	return ptr;
}

/* enqueue a node if the queue is not full */
void enqueue(struct array_queue *queue, int data)
{
	// pthread_mutex_lock(&queue->lock);

	/* check if queue is full*/
	if (queue->count >= queue->capacity)
	{
		perror("queue is full");
		// pthread_cond_wait(&(queue->not_full), &(queue->lock));
	}

	/* enqueue */
	if (queue->count <= 0)
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
		queue->array[(queue->tail)+1] = data;
		queue->tail++;
		queue->count++;	
	}

	printf("en queue count: %d\n", queue->count);

	// pthread_cond_signal(&queue->not_empty);
	// pthread_mutex_unlock(&queue->lock);
}

/* dequeue the head of the queue */
int dequeue(struct array_queue *queue)
{
	int head, data;

	// pthread_mutex_lock(&queue->lock);
	/* if the queue is empty, wait*/
	if (queue->count <= 0)
	{
		perror("queue is empty");
		// pthread_cond_wait(&(queue->not_empty), &(queue->lock));
	}

	/* dequeue */
	head = queue->head;
	data = queue->array[head];

	/* update */
	if (queue->head == queue->capacity - 1)
	{
		queue->head = 0;
		queue->count--;
	}
	else
	{
		queue->head++;
		queue->count--;
	}

	printf("de queue count: %d\n", queue->count);

	// pthread_cond_signal(&(queue->not_full));
	// pthread_mutex_unlock(&queue->lock);

	return data;
}

/* get the size of */
int get_size(struct array_queue *queue)
{
	// pthread_mutex_lock(&queue->lock);

	int size = queue->count;

	// pthread_mutex_unlock(&queue->lock);

	return size;
}

/* display elements in  queue */
void display_queue(struct array_queue *queue)
{
	int i;
	for (i = 0; i < queue->capacity; ++i)
	{
		/* code */
		printf("%d ", queue->array[i]);
	}
	printf("\n");
}

/* destroy queue */
void destroy_queue(struct array_queue *queue)
{
	free(queue->array);
	free(queue);
}