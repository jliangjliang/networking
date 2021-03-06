#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>

void *server_handler(void *fd_pointer);

int main()
{
	// variable (df: file decriptor)
	int listenSocket, connfd, *clitSock;

	// create a listen socket
	listenSocket = socket(AF_INET, SOCK_STREAM, 0);

	puts("socket created.");

	// sock add
	struct sockaddr_in servAddr, clitAddr;

	// zero out the servAddr
	memset(&servAddr, '\0', sizeof(servAddr));

	// configure servAddr
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = INADDR_ANY;
	servAddr.sin_port = htons(1234);

	// bind()
	bind(listenSocket, (struct sockaddr *) &servAddr, sizeof(servAddr));

	puts("binded");

	// listen
	listen(listenSocket, 5);

	puts("waiting for connections.");

	// socklen
	socklen_t clitlen;

	clitlen = sizeof(clitAddr);

	while (connfd = accept(listenSocket, (struct sockaddr *) &clitAddr, &clitlen))
	{
		puts("Connection accepted");

		// create thread, need to keep track, pthread destroy
		pthread_t serverThread;

		// malloc will corrr
		// clitSock = malloc(1);

		// *clitSock = connfd;

		// clitSock as a arg will be passthrough the server_handler as the start_routine in the pthread_create
		pthread_create(&serverThread, NULL, server_handler, (void *) connfd);

	}


}



// void *server_handler(int connfd)
void *server_handler(void *fd_pointer)
{

	// link sock to the fd_pointer
	// int sock = *(int *) fd_pointer;
	int sock = (int) fd_pointer;

	// file variable
	char *file_path = "text0.txt";
	int filefd;

	// open a file
	filefd = open(file_path, O_RDONLY);
	if ( filefd == -1)
	{
		perror("file open failed");
		exit(EXIT_FAILURE);
	}

	int recv_size, send_size;

	char recv_buffer[2000];

	while ((recv_size = recv(sock, recv_buffer, 2000, 0)) > 0)
	{

		recv_buffer[recv_size] = '\0';

		printf("recv_size: %d\n", recv_size);
		printf("recv: %s\n", recv_buffer);


		if (strcmp(recv_buffer, "text0.txt") == 0)
		{

			send_size = send(sock, recv_buffer, strlen(recv_buffer), 0);

			printf("send_size: %d\n", send_size);

			printf("Resource name: %s\n", recv_buffer);

			recv_size = read(filefd, recv_buffer, 2000);

			recv_buffer[recv_size] = '\0';

			printf("read: %s\n", recv_buffer);

			if (recv_size == -1)
			{
				perror("read file failed");
				exit(EXIT_FAILURE);
			}

			send_size = send(sock, recv_buffer, recv_size, 0);

			printf("send: %s\n", recv_buffer);

			if (send_size == -1)
			{
				perror("send file failed");
				exit(EXIT_FAILURE);
			}

			printf("File sent\n");


		} else {
			send_size = send(sock, "Resource Not Found", strlen("Resource Not Found"), 0);

			printf("send_size: %d\n", send_size);

			printf("message send: %s\n", "Resource Not Found");
		}

		lseek(filefd, 0, SEEK_SET);

	}

	close(filefd);
	close(sock);

	free(fd_pointer);

	return 0;
}