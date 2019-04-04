#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main()
{

	int sockfd;
	ssize_t recv_size, send_size;
	char buffer[2000];
	char server_reply[2000];

	ssize_t n;

	struct sockaddr_in servaddr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1)
		perror("socket() failed");

	printf("Created Socket\n");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(8080);

	connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

	while(1)
	{
		printf("Enter a Message:\n");
		
		scanf("%s", buffer);

		send_size = send(sockfd, buffer, strlen(buffer), 0);

		recv_size = recv(sockfd, server_reply, 2000, 0);

		server_reply[recv_size] = '\0'; 

		printf("Server Reply: %s \n\n", server_reply);

	}

	close(sockfd);
	return 0;

}