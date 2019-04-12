#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <syscall.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include "array_queue_fork.h"

#define NUMWORKER 2
#define PORT 25969
#define LISTENSIZE 2
#define BUFFERSIZE 2000
#define QUEUECAPACITY 2
#define MEMSIZE 1024

// TODO: need to destroy buffer, queue, thread

/* declare functions */
void *clientHandler(void *fdPointer);

int main()
{
	/* Share memory */

	/* create access key */
	key_t key = ftok("shmfile", 65);

	/* get share memory id */
	int shmid = shmget(key, MEMSIZE, IPC_CREAT | 0666);

	/* declare shm */
	struct array_queue *shm;

	/* attach to share memory */
	shm = (struct array_queue *) shmat(shmid, NULL, 0);

	// int protection = PROT_READ | PROT_WRITE;

	// int visibility = MAP_ANONYMOUS | MAP_SHARED;

	// void* shm = mmap(NULL, 128, protection, visibility, 0, 0);

	/* create connection queue */
	struct array_queue *connQueue = create_queue(QUEUECAPACITY);

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

	/* connfd */
	int connfd;

	// Fork, a parent and a child
	int pid;
	pid = fork();

	if (pid > 0)
	{
		printf("parent pid: %d\n", getpid());
		memcpy(shm, connQueue, sizeof(struct array_queue));
		enqueue(shm, 4);
		display_queue(shm);
		display_queue(connQueue);
		// printf("%s\n", shm);
		/* loop the server, accept and enqueue connection */
		// while (1)
		// {
		// 	connfd = accept(listenfd, (struct sockaddr *) &clitAddr, &clitlen);
		// 	if (connfd > 0)
		// 	{
		// 		enqueue(connQueue, connfd);
		// 	}
		// 	printf("enqueue connfd: %d\n", connfd);
		// }
	}
	else if (pid == 0)
	{
		// sleep(1);
		printf("child pid: %d\n", getpid());
		display_queue(shm);
		display_queue(connQueue);
		// clientHandler((void *) connQueue);
	}
	else
	{
		perror("fork() failed");
		exit(1);
	}

	return 0;
}

/* client_handler */
void *clientHandler(void *connQueue)
{
	/* variables */
	struct array_queue *queue = (struct array_queue *) connQueue;
	int read_size, write_size;
	char buffer[BUFFERSIZE];
	int sockfd = -1;

	puts("start handling\n");

	/* loop, dequeue, read and write */
	while (1)
	{	
		if (sockfd == -1)
		{
			printf("sockfd before dequeue: %d\n", sockfd);
			sockfd = dequeue(queue);
			printf("sockfd after dequeue: %d\n", sockfd);
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
}
