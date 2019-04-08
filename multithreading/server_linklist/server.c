#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "queue.h"

#define NUMWORKER 2
#define PORT 25969
#define LISTENSIZE 2
#define BUFFERSIZE 2000
#define QUEUECAPACITY 2

// TODO: need to destroy buffer, queue, thread

/* declare functions */
void *clientHandler(void *fdPointer);

int main()
{
	/* create connection queue */
	struct Queue *connQueue = create_queue(QUEUECAPACITY);

	/* server address */
	struct sockaddr_in servAddr;
	memset(&servAddr, '\0', sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = INADDR_ANY;
	servAddr.sin_port = htons(PORT);

	/* client address */
	struct sockaddr_in clitAddr;
	socklen_t clitlen;
	clitlen = sizeof(clitAddr);

	/* create, bind and listen listenfd */
	int listenfd;
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	bind(listenfd, (struct sockaddr *)&servAddr, sizeof(servAddr));
	listen(listenfd, NUMWORKER);

	/* create working threads */
	pthread_t workerThread[NUMWORKER];
	for (int i = 0; i < NUMWORKER; ++i)
	{
		pthread_create(&workerThread[i], NULL, clientHandler, (void *) connQueue);
	}

	puts("working threads created.");

	int connfd;

	/* loop the server, accept and enqueue connection */
	while (1)
	{
		connfd = accept(listenfd, (struct sockaddr *) &clitAddr, &clitlen);
		if (connfd > 0)
		{
			enqueue(connQueue, connfd);
		}

	}

	return 0;
}

/* client_handler */
void *clientHandler(void *connQueue)
{
	/* variables */
	struct Queue *queue = (struct Queue *) connQueue;
	int read_size, write_size;
	char buffer[BUFFERSIZE];
	int sockfd = -1;
	struct Node *head;

	puts("start handling\n");

	/* loop, dequeue, read and write */
	while (1)
	{	
		if (sockfd == -1)
		{
			head = dequeue(queue);
			sockfd = head->data;
		}

		if (sockfd > 0)
		{
			/* read sockfd */
			read_size = read(sockfd, buffer, BUFFERSIZE);

			/* if connection closed, reset sockfd*/
			if (read_size == 0)
			{
				sockfd = -1;
				continue;
			}

			/* write sockfd */
			buffer[read_size] = '\0';
			write_size = write(sockfd, buffer, read_size);
		}
	}

	close(sockfd);
	return 0;
}
