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
#include <sys/sem.h>
#include <sys/ipc.h>
#include <string.h>

union semun
{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};


void *handleClient(void *args)
{
	int clifd = *(int *)args;
	int semid = *(int *)(args + 4);
	
	// account type
	int accountType = 0, userID = -1; // if Admin -> 1, if user -> 2
	char userAdminBuff[MAX_MESSAGE_LEN];
	read(clifd, userAdminBuff, 5);
	accountType = userAdminBuff[0] - '0';
	userID = atoi(userAdminBuff + 2);
	printf("userID = %d\n", userID);
	
	while (1)
	{
		char request[MAX_MESSAGE_LEN], response[MAX_MESSAGE_LEN];
		read(clifd, request, MAX_MESSAGE_LEN);
		printf("client -> %s", request);
		if (strcmp(request, "EXITEXIT"))
		{
			printf("Client Quiting...\n");
			close(clifd);
			break;
		}
		handleClientRequest(request, response, accountType, clifd, userID);
		write(clifd, response, MAX_MESSAGE_LEN);
	}
	
	struct sembuf buf = {0, 1, 0};
	int sct = semop(semid, &buf, 1);
	printf("Releasing Semaphore -> %d\n", sct);
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
	
	union semun arg;
	int key = ftok(".", 'a');
	int semid = semget(key, 1, IPC_CREAT | 0777);
	arg.val = MAX_CONCURRENT_CONN;
	int st = semctl(semid, 0, SETVAL, arg);
	if (!st) printf("Semaphore Initialized...\n");
	printf("Traffic Limited to %d connections...\n", arg.val);

	
		
	char msgMain[MAX_MESSAGE_LEN] = "Welcome to Ecommerce service:\nLogin as:\n1. Admin\n2. User(if User, enter : 2 UserID \n";
	
	int clientAddrLen = sizeof(client);
	
	int iterations = 0;
	
	
	
	while (1)
	{
		struct sembuf buf = {0, -1, 0};
		int sct = semop(semid, &buf, 1);
		printf("Loking Sem -> %d", sct);
		
		int clifd = accept(sockfd, (struct sockaddr *)&client, &clientAddrLen);
		perror("accept");
		if (clifd == -1)
		{
			printf("Connection with Client failed.\n");
			continue;
		}
		write(clifd, msgMain, MAX_MESSAGE_LEN);
		pthread_t thread;
		
		int arr[2] = {clifd, semid};
		pthread_create(&thread, NULL, handleClient, arr);
		
		pthread_detach(thread);
	}
	// removing semaphore
	semctl(semid, 0, IPC_RMID, arg);
	close(sockfd);
}
	
	
	
