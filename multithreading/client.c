#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define BUFFERSIZE 2000
#define PORT 25969

int main()
{
	// variable
	int sockdf;
	int recv_size, write_size;
	struct sockaddr_in servAddr;
	char buffer[BUFFERSIZE];

	// create socket
	sockdf = socket(AF_INET, SOCK_STREAM, 0);

	puts("sock created.\n");

	// configure servaddr
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servAddr.sin_port = htons(PORT);

	// connect
	connect(sockdf, (struct sockaddr *)& servAddr, sizeof(servAddr));

	while(1)
	{
		// keyboard input
		printf("Please enter something\n");
		scanf("%s", buffer);

		// write
		write_size = write(sockdf, buffer, strlen(buffer));
		printf("write_size: %d\n", write_size);
		printf("write: %s\n\n", buffer);

		// read
		recv_size = read(sockdf, buffer, BUFFERSIZE);
		buffer[recv_size] = '\0';
		printf("recv_size: %d\n", recv_size);
		printf("recv: %s\n\n", buffer);
	}

	close(sockdf);
	return 0;
}