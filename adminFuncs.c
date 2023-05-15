#include "adminFuncs.h"
#include "protocol.h"
#include "Product.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int max(int a, int b)
{
	return (a >= b ? a : b);
}

void adminOperationADDDEL(char cReq[][MAX_WORD_SZ], char *response)
{
	int fd_prod = open(PRODUCT_FILE, O_CREAT | O_RDWR, 0777);
	printf("fdprod : %d\n", fd_prod);
	if (!strcmp(cReq[1], "0"))
	{
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence = SEEK_CUR;
		lock.l_start = 0;
		lock.l_len = 0;
		lock.l_pid = getpid();
		
		fcntl(fd_prod, F_SETLKW, &lock);
		perror("Inside critical section\n");
	
		struct Product newProd;
		
		int newProdID = getNewID(fd_prod);
		
		newProd.product_id = newProdID;
		strcpy(newProd.product_name, cReq[2]);
		newProd.cost = atoi(cReq[3]);
		newProd.quantity = atoi(cReq[4]);
		
		lseek(fd_prod, 0, SEEK_END);
		write(fd_prod, &newProd, sizeof(newProd));
		perror("write");
		
		lock.l_type = F_UNLCK;
		fcntl(fd_prod, F_SETLK, &lock);
		perror("Outside Critical Section\n");
		
		close(fd_prod);
		strcpy(response, "The Product is Added.\n");
	}
	else if (!strcmp(cReq[1], "1"))
	{
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence = SEEK_CUR;
		lock.l_start = 0;
		lock.l_len = 0;
		lock.l_pid = getpid();
		
		fcntl(fd_prod, F_SETLKW, &lock);
		perror("Inside critical section\n");
		
		int prodIDToDel = atoi(cReq[2]);
		struct Product prodList[MAX_PRODUCTS];
		
		lseek(fd_prod, 0, SEEK_SET);
		int iter = 0;
		while(read(fd_prod, &prodList[iter], sizeof(struct Product)) > 0)
		{
			iter++;
		}
		
		close(fd_prod);
		fd_prod = open(PRODUCT_FILE, O_RDWR | O_TRUNC, 0777);
		
		for (int itemNo = 0; itemNo < iter; itemNo++)
		{
			if (prodList[itemNo].product_id == prodIDToDel)
				continue;
			
			write(fd_prod, &prodList[itemNo], sizeof(struct Product));
		}
		
		lock.l_type = F_UNLCK;
		fcntl(fd_prod, F_SETLK, &lock);
		perror("Outside Critical Section\n");
		
		close(fd_prod);
		strcpy(response, "The Product with specified ID is deleted!\n");
	}

}

void adminOperationUPDATE(char cReq[][MAX_WORD_SZ], char *response)
{
	int prodIDToUpd = atoi(cReq[1]);
	int prodCost = atoi(cReq[2]);
	int prodQty = atoi(cReq[3]);
	
	int fd_prod = open(PRODUCT_FILE, O_RDWR | O_CREAT, 0777);
	
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_CUR;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_pid = getpid();
	
	fcntl(fd_prod, F_SETLKW, &lock);
	perror("Inside critical section\n");

	struct Product prod;
	while (read(fd_prod, &prod, sizeof(struct Product)) > 0)
	{
		if (prod.product_id == prodIDToUpd)
		{
			prod.cost = prodCost;
			prod.quantity = prodQty;
		}
		lseek(fd_prod, -sizeof(struct Product), SEEK_CUR);
		write(fd_prod, &prod, sizeof(struct Product));
		break;
	}
	
	lock.l_type = F_UNLCK;
	fcntl(fd_prod, F_SETLK, &lock);
	perror("Outside Critical Section\n");
	
	close(fd_prod);
	strcpy(response, "Product List is updated!\n");
}


int getNewID(int fd_prod)
{
	struct Product prod;
	int maxProdId = 1;
	while(read(fd_prod, &prod, sizeof(struct Product)) > 0)
	{
		maxProdId = max(prod.product_id, maxProdId);
	}
	return (maxProdId + 1);
}
