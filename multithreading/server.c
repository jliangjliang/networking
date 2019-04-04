#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/queue.h>
#include "queue.h"

#define NUMWORKER 2
#define PORT 25969
#define LISTENSIZE 2
#define BUFFERSIZE 2000

// declare functions
void *clientHandler(void *fdPointer);

// mutex lock
pthread_mutex_t lock;

int main()
{
	// create connection queue
	struct Queue *connQueue = createQueue();

	// server address
	struct sockaddr_in servAddr;
	memset(&servAddr, '\0', sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = INADDR_ANY;
	servAddr.sin_port = htons(PORT);

	// client address
	struct sockaddr_in clitAddr;
	socklen_t clitlen;
	clitlen = sizeof(clitAddr);

	// create, bind and listen listenfd
	int listenfd;
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	bind(listenfd, (struct sockaddr *)&servAddr, sizeof(servAddr));
	listen(listenfd, NUMWORKER);

	// create working threads
	pthread_t workerThread[NUMWORKER];
	for (int i = 0; i < NUMWORKER; ++i)
	{
		pthread_create(&workerThread[i], NULL, clientHandler, (void *) connQueue);
	}

	puts("working threads created.");

	// loop the server
	while (1)
	{
		if (connQueue->count <= NUMWORKER)
		{
			// put connection to array (assume connection is not a problem)
			pthread_mutex_lock(&lock);
			enqueue(connQueue, (accept(listenfd, (struct sockaddr *) &clitAddr, &clitlen)));
			pthread_mutex_unlock(&lock);
			printf("Queue size: %d\n", connQueue->count);
		}

	}

	return 0;
}

// client_handler
void *clientHandler(void *connQueue)
{
	
	struct Queue *queue = (struct Queue *) connQueue;
	int read_size, write_size;
	char buffer[BUFFERSIZE];
	int sockfd;
	struct Node *head;

	puts("start handling");

	// loop the queue
	while (1)
	{
		// get connection, need mutext
		pthread_mutex_lock(&lock);
		if (queue->count > 0)
		{
			head = dequeue(queue);
		}
		pthread_mutex_unlock(&lock);

		sockfd = head->data;

		// read sockfd
		read_size = read(sockfd, buffer, BUFFERSIZE);
		printf("read_size: %d\n", read_size);
		printf("read: %s\n", buffer);

		// write sockfd
		buffer[read_size] = '\0';
		write_size = write(sockfd, buffer, read_size);
		printf("write_size: %d\n", write_size);
		printf("write: %s\n", buffer);
	}

	return 0;
}
