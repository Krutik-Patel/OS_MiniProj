#include "requestHandler.h"
#include "userFuncs.h"
#include "adminFuncs.h"
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>


void handleClientRequest(char* request, char*response, int accountType, int cfd, int userID)
{
	parseResponse(request);
	if (accountType == 1)
	{
		printf("%s\n", currRequest[0]);
		if (!strcmp(currRequest[0], "1"))
		{
			int logfd = open("AdminLog.log", O_CREAT | O_WRONLY, 0777);
			lseek(logfd, 0, SEEK_END);
			adminOperationADDDEL(currRequest, response);
			char msgMain[MAX_MESSAGE_LEN];
			sprintf(msgMain, "ADD/DEL REQUEST: %s\n", request);
			write(logfd, msgMain, sizeof(msgMain));
			close(logfd);
			
		}
		else if (!strcmp(currRequest[0], "2"))
		{
			int logfd = open("AdminLog.log", O_CREAT | O_WRONLY, 0777);
			lseek(logfd, 0, SEEK_END);
			adminOperationUPDATE(currRequest, response);
			char msgMain[MAX_MESSAGE_LEN];			
			sprintf(msgMain, "UPDATE REQUEST: %s\n", request);
			write(logfd, msgMain, sizeof(msgMain));
			close(logfd);
		}
		else if (!strcmp(currRequest[0], "3"))
		{
			int logfd = open("AdminLog.log", O_CREAT | O_WRONLY, 0777);
			lseek(logfd, 0, SEEK_END);
			userListProducts(response);
			char msgMain[MAX_MESSAGE_LEN];
			sprintf(msgMain, "ADMIN EXIT\n");
			write(logfd, msgMain, sizeof(msgMain));
			close(logfd);
		}
		
	}
	else if (accountType == 2)
	{
		if (!strcmp(currRequest[0], "1"))
		{
			int logfd = open("AdminLog.log", O_CREAT | O_WRONLY, 0777);
			lseek(logfd, 0, SEEK_END);
			userListProducts(response);
			char msgMain[MAX_MESSAGE_LEN];
			sprintf(msgMain, "VIEW_PROD_REQ USER_ID: %d REQUEST: %s\n", userID, request);
			write(logfd, msgMain, sizeof(msgMain));
			close(logfd);
		}
		else if (!strcmp(currRequest[0], "2"))
		{
			int logfd = open("AdminLog.log", O_CREAT | O_WRONLY, 0777);
			lseek(logfd, 0, SEEK_END);
			userListCart(response, userID);
			char msgMain[MAX_MESSAGE_LEN];
			sprintf(msgMain, "VIEW_CART_REQ USER_ID: %d REQUEST: %s\n", userID, request);
			write(logfd, msgMain, sizeof(msgMain));
			close(logfd);
		}
		else if (!strcmp(currRequest[0], "3"))
		{
			int logfd = open("AdminLog.log", O_CREAT | O_WRONLY, 0777);
			lseek(logfd, 0, SEEK_END);
			addToCart(currRequest, response, userID);
			char msgMain[MAX_MESSAGE_LEN];
			sprintf(msgMain, "ADD_CART USER_ID: %d REQUEST: %s\n", userID, request);
			write(logfd, msgMain, sizeof(msgMain));
			close(logfd);
		}
		else if (!strcmp(currRequest[0], "4"))
		{
			int logfd = open("AdminLog.log", O_CREAT | O_WRONLY, 0777);
			lseek(logfd, 0, SEEK_END);
			editCart(currRequest, response, userID);
			char msgMain[MAX_MESSAGE_LEN];
			sprintf(msgMain, "MODIFY_CART USER_ID: %d REQUEST: %s\n", userID, request);
			write(logfd, msgMain, sizeof(msgMain));
			close(logfd);
		}
		else if (!strcmp(currRequest[0], "5"))
		{
			int logfd = open("AdminLog.log", O_CREAT | O_WRONLY, 0777);
			lseek(logfd, 0, SEEK_END);
			buyCart(request, response, cfd, userID);
			char msgMain[MAX_MESSAGE_LEN];	
			sprintf(msgMain, "BUY_REQ USER_ID: %d REQUEST: %s\n", userID, request);
			write(logfd, msgMain, sizeof(msgMain));
			close(logfd);
		}
	}
	
}

void parseResponse(char *request)
{
	// cleaning request words array
	for (int i = 0; i < MAX_REQ_WORDS; i++)
		for (int j = 0; j < MAX_WORD_SZ; j++)
			currRequest[i][j] = '\0';

	int wordNo = 0, charNo = 0, i = 0;
	
	// splitting into different words
	while(request[i])
	{
		if (request[i] == ' ')
		{
			currRequest[wordNo][charNo] = '\0';
			charNo = 0;
			wordNo++;
			i++;
			continue;
		}
		currRequest[wordNo][charNo++] = request[i++];
	}
	currRequest[++wordNo][0] = '\0';
}


















