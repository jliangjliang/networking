#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080

int main()
{
	int server_df, client_socket, status_bind, status_listen;
	long valread, valwrite;
	struct sockaddr_in server_address;
	int addrlen = sizeof(server_address);
	char *hello = "Hello from server";

	//create socket
	server_df = socket(AF_INET, SOCK_STREAM, 0);
	if (server_df == 0)
	{
		perror("In socket");
		exit(EXIT_FAILURE);
	}

	//configure
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(PORT);
	memset(server_address.sin_zero, '\0', sizeof(server_address.sin_zero));

	//bind
	status_bind = bind(server_df, (struct sockaddr *)&server_address, addrlen);
	if (status_bind < 0)
	{
		perror("In bind");
		exit(EXIT_FAILURE);
	}

	//listen
	status_listen = listen(server_df, 10);
	if (status_listen < 0)
	{
		perror("In listen");
		exit(EXIT_FAILURE);
	}

	while(1)
	{
		printf("\nWaiting for new connection\n\n");
		client_socket = accept(server_df, (struct sockaddr *)&server_address, (socklen_t *)&addrlen);
		if (client_socket < 0)
		{
			perror("In accept");
			exit(EXIT_FAILURE);
		}
		char buffer[30000] = {0};
		valread = read(client_socket, buffer, 30000);
		printf("%s\n", buffer);
		valwrite = write(client_socket, hello, strlen(hello));
		printf("Hello message sent\n");
		close(client_socket);
	}
	return 0;
}