#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include "requestHandler.h"

#define MAX_CONN_BACKLOG 5
#define MAX_MESSAGE_LEN 1000
#define PORT_NO 10000

void *handleClient(void *args)
{
	int clifd = *(int *)args;
	
	while (1)
	{
		char request[MAX_MESSAGE_LEN], response[MAX_MESSAGE_LEN];
		read(clifd, request, MAX_MESSAGE_LEN);
		strcpy(response, handleClientRequest(request));
		write(clifd, response, MAX_MESSAGE_LEN);
	}
	close(clifd);
}


int main()
{
	struct sockaddr_in server, client;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT_NO);
	server.sin_family = AF_INET;
	
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	bind(sockfd, (struct sockaddr *) &server, sizeof(server));
	perror("Binding with socket... ");
	listen(sockfd, MAX_CONNS);
	printf("Listening from server...\n");
	
	char msgMain[MAX_MESSAGE_LEN] = "Welcome to ___ service\n";
	int clientAddrLen = sizeof(client);
	
	
	
	while (1)
	{
		int clifd = accept(sockfd, (struct sockaddr *)&client, &clientAddrLen);
		if (accept == -1)
		{
			printf("Connection with Client failed.\n");
			continue;
		}
		pthread_t thread;
		pthread_create(thread, NULL, handleClient, &clifd);
		
		pthread_detach(thread);
	}
	close(sockfd);
}
	
	
	
