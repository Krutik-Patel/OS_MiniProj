#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT_NO 10000
#define TARGET_IP "127.0.0.1"
#define MAX_MESSAGE_LEN 1000

int main()
{
	struct sockaddr_in server;
	server.sin_addr.s_addr = inet_addr(TARGET_IP);
	server.sin_port = htons(PORT_NO);
	server.sin_family = AF_INET;
	
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	int connect(sockfd, (struct sockaddr *) &server, sizeof(server));
	perror("Connection with Server");
	char buffmain[MAX_MESSAGE_LEN];
	read(sockfd, buffmain, MAX_MESSAGE_LEN);
	printf("SERVER :  %s", buffmain);
	
	while (1)
	{
		char request[MAX_MESSAGE_LEN] = handleInput(), serverResponse[MAX_MESSAGE_LEN];
		// response considered only of one integer for now.
		if (strcmp(request, terminationStr)
		{
			printf("Exiting application...\n");
			break;
		}
		write(sockfd, request, MAX_MESSAGE_LEN);
		read(sockfd, serverResponse, MAX_MESSAGE_LEN);
		printf("SERVER : %d", serverResponse);
	}
	close(sockfd);
	perror("Connection to server closing... ");
}
	
	
	
