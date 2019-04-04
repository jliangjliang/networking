#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

void *server_handler(void *fd_pointder);

int main()
{
	int listenfd, connfd, *new_sock;
	socklen_t clilen;
	struct sockaddr_in cliaddr, servaddr;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	if (listenfd == -1)
		perror("socket() failed");

	puts("socket created");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(8080);

	if (bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
	{
		perror("bind() failed");
		return 1;
	}

	puts("bind() success");

	listen(listenfd, 10);

	puts("waiting for connections");
	clilen = sizeof(cliaddr);

	while ((connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen)))
	{
		puts("Connection accepted");

		pthread_t server_thread;
		new_sock = malloc(1);
		*new_sock =  connfd;
		pthread_create(&server_thread, NULL, server_handler, (void*) new_sock);
	}

	if (connfd < 0)
	{
		perror("accept() failed");
		return 1;
	}

	return 0;
}

void *server_handler(void *fd_pointer)
{
	printf("Hello Server Handler \n");
	int sock = *(int *)fd_pointer;

	int read_size, write_size;
	char *message;
	char client_message[2000];
	message = " \nHello Server Handler \n";

	static int send_once = 0;
	if (send_once < 1)
		send_once++;

	while((read_size = read(sock, client_message, 2000)) > 0)
	{
		printf("Read Size %d \n", read_size);
		printf("read message size: %lu\n", strlen(client_message));
		printf("message recived: %s\n", client_message);
		write(sock, client_message, strlen(client_message));
		printf("message sent: %s\n\n", client_message);
		memset(client_message, 0, sizeof(client_message));
	}

	if(read_size == 0)
	{
		puts("client disconnected");
		fflush(stdout);
	}
	else if(read_size == -1)
	{
		perror("recv() failed");
	}
	free(fd_pointer);

	return 0;
}