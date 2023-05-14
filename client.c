#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include "protocol.h"

char terminationStr[MAX_MESSAGE_LEN] = "EXIT";
int accountType;

void handleInput(char *request)
{
	if (accountType == 1)
	{
		printf("Select one and enter the corresponding number and required arguments:\n1. Add or Delete a product.\n2. Update quantity/price of a product.\n");
		int requestType;
		scanf("%d", &requestType);
		if (requestType == 1)
		{
			strcpy(request, "1 ProductID Operation");
		}
		else if (requestType == 2)
		{
			strcpy(request, "2 ProductID Price Quantity");
		}
	}
	else if (accountType == 2)
	{
		printf("Select one and enter the corresponding number and required arguments:\n1. List Available Products.\n2. Display Cart.\n3. Add a Product to cart.\n4. Edit Cart items.\n");
		int requestType;
		scanf("%d", &requestType);
		if (requestType == 1)
		{
			strcpy(request, "1");
		}
		else if (requestType == 2)
		{
			strcpy(request, "2");
		}
		else if (requestType == 3)
		{
			strcpy(request, "3 ProductID Quantity");
		}
		else if (requestType == 4)
		{
			strcpy(request, "4 ProductID Quantity");
		}
	}	
}

int main()
{
	struct sockaddr_in server;
	server.sin_addr.s_addr = inet_addr(TARGET_IP);
	server.sin_port = htons(PORT_NO);
	server.sin_family = AF_INET;
	
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	connect(sockfd, (struct sockaddr *) &server, sizeof(server));
	perror("Connection with Server");
	char buffmain[MAX_MESSAGE_LEN];
	read(sockfd, buffmain, MAX_MESSAGE_LEN);
	
	// setting and sending accountType
	printf("SERVER :  %s", buffmain);
	scanf("%d", &accountType);
	char accBuff[1];
	accBuff[0] = '0' + accountType;
	write(sockfd, accBuff, 1);
	
	
	while (1)
	{
		char request[MAX_MESSAGE_LEN], serverResponse[MAX_MESSAGE_LEN];
		handleInput(request);
		// response considered only of one integer for now.
		if (!strcmp(request, terminationStr))
		{
			printf("Exiting application...\n");
			break;
		}
		write(sockfd, request, MAX_MESSAGE_LEN);
		read(sockfd, serverResponse, MAX_MESSAGE_LEN);
		printf("SERVER : %s", serverResponse);
	}
	close(sockfd);
	perror("Connection to server closing... ");
}
	
	
	
