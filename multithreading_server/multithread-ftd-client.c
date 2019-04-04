#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

int main()
{
	// variable
	int sockdf;
	int recv_size, send_size, write_size;
	struct sockaddr_in servAddr;
	char send_buffer[2000];
	char recv_buffer[2000];

	// filename
	int num = 0;
	char filename[10];
	sprintf(filename, "out_%d.txt", num);

	// file variable
	char *file_path = filename;
	int filefd;

	// create socket
	sockdf = socket(AF_INET, SOCK_STREAM, 0);

	puts("sock created.");

	// configure servaddr
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servAddr.sin_port = htons(1234);

	// connect
	connect(sockdf, (struct sockaddr *)& servAddr, sizeof(servAddr));

	// open file
	filefd = open(file_path, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (filefd == -1)
	{
		perror("open file failed");
		exit(EXIT_FAILURE);
	}

	while(1)
	{
		printf("Please enter the fail name you want to retrive\n");

		scanf("%s", send_buffer);

		send_size = write(sockdf, send_buffer, strlen(send_buffer));

		printf("send_size: %d\n", send_size);
		printf("send: %s\n\n", send_buffer);

		recv_size = read(sockdf, recv_buffer, 2000);

		recv_buffer[recv_size] = '\0';

		printf("recv_size: %d\n", recv_size);
		printf("recv: %s\n", recv_buffer);

		printf("Server Reply: %s \n\n", recv_buffer);

		if (strcmp(recv_buffer, send_buffer) == 0)
		{
			printf("%s\n", "great");

			recv_size = read(sockdf, recv_buffer, 2000);

			printf("recv_size: %d\n", recv_size);

			recv_buffer[recv_size] = '\0';

			write_size = write(filefd, recv_buffer, recv_size);

			printf("write_size: %d\n", write_size);

			if (write_size == -1)
			{
				perror("write to file failed");
				exit(EXIT_FAILURE);
			}


		}

	}

	close(sockdf);
	close(filefd);
	return 0;
}