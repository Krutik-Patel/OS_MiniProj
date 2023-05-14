#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include "requestHandler.h"
#include "protocol.h"


void *handleClient(void *args)
{
	int clifd = *(int *)args;
	
	// account type
	int accountType = 0; // if Admin -> 1, if user -> 2
	char userAdminBuff[MAX_MESSAGE_LEN];
	read(clifd, userAdminBuff, 1);
	accountType = userAdminBuff[0] - '0';
		
	
	while (1)
	{
		char request[MAX_MESSAGE_LEN], response[MAX_MESSAGE_LEN];
		read(clifd, request, MAX_MESSAGE_LEN);
		printf("client -> %s", request);
		handleClientRequest(request, response, accountType);
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
	listen(sockfd, MAX_CONN_BACKLOG);
	perror("listen"); 
	printf("Listening from server...\n");
	
		
	char msgMain[MAX_MESSAGE_LEN] = "Welcome to Ecommerce service:\nLogin as:\n1. Admin\n2.User\n";
	
	int clientAddrLen = sizeof(client);
	
	int iterations = 0;
	
	while (1)
	{
		int clifd = accept(sockfd, (struct sockaddr *)&client, &clientAddrLen);
		perror("accept");
		if (clifd == -1)
		{
			printf("Connection with Client failed.\n");
			continue;
		}
		write(clifd, msgMain, MAX_MESSAGE_LEN);
		pthread_t thread;
		pthread_create(&thread, NULL, handleClient, &clifd);
		
		pthread_detach(thread);
	}
	close(sockfd);
}
	
	
	
