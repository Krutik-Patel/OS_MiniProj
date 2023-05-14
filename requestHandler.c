#include "requestHandler.h"
#include <string.h>
#include <stdio.h>
void handleClientRequest(char* request, char*response, int accountType)
{
	parseResponse(request);
	if (accountType == 1)
	{
		printf("%s\n", currRequest[0]);
		if (!strcmp(currRequest[0], "1"))
		{
			printf("Add or delete product\n");
		}
		else if (!strcmp(currRequest[0], "2"))
		{
			printf("Update product\n");
		}
	}
	else if (accountType == 2)
	{
		if (!strcmp(currRequest[0], "1"))
		{
			printf("Add or delete product\n");
		}
		else if (!strcmp(currRequest[0], "2"))
		{
			printf("Update product\n");
		}
		else if (!strcmp(currRequest[0], "3"))
		{
			printf("Cart add\n");
		}
		else if (!strcmp(currRequest[0], "4"))
		{
			printf("Edit card\n");
		}
	}
	strcpy(response, "You response has been responded properly\n");
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


















