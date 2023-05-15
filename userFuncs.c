#include "userFuncs.h"
#include "Product.h"
#include "protocol.h"
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

void userListProducts(char *response)
{
	char msgToSend[MAX_MESSAGE_LEN];
	int fd = open(PRODUCT_FILE, O_CREAT | O_RDWR, 0777);
	perror("UserList open file");
	struct Product prod;
	
	lseek(fd, 0, SEEK_SET);
	while(read(fd, &prod, sizeof(prod)) > 0)
	{
		char result[30];
		sprintf(result, "%d ", prod.product_id);
		strcat(msgToSend, result);
		strcat(msgToSend, prod.product_name);
		sprintf(result, " %d ", prod.cost);
		strcat(msgToSend, result);
		sprintf(result, " %d\n", prod.quantity);
		strcat(msgToSend, result);
		printf("%s\n", msgToSend);		
	}
	strcpy(response, "\n::PRODUCT LIST::\nProduct Info in order: P_ID, P_NAME, COST, QTY\n");
	strcat(response, msgToSend);
	printf("response : %s", response);
	strcpy(msgToSend, "");
	close(fd);
}

void userListCart(char *response)
{
	char msgToSend[MAX_MESSAGE_LEN];
	int fd = open("ID.cart", O_RDWR, 0777);
	struct Product prod;
	
	lseek(fd, 0, SEEK_SET);
	while(read(fd, &prod, sizeof(struct Product)) > 0)
	{
		char result[30];
		sprintf(result, "%d ", prod.product_id);
		strcat(msgToSend, result);
		strcat(msgToSend, prod.product_name);
		sprintf(result, " %d ", prod.cost);
		strcat(msgToSend, result);
		sprintf(result, " %d\n", prod.quantity);
		strcat(msgToSend, result);
		
		strcpy(response, "::CART ITEMS::\nProduct Info in order: P_ID, P_NAME, COST, QTY\n");
		strcat(response, msgToSend);
	}
	
	close(fd);
}

void addToCart(char cReq[][MAX_WORD_SZ], char *response)
{
	int fd_cart = open("ID.cart", O_RDWR, 0777);
	int fd_prodList = open(PRODUCT_FILE, O_CREAT | O_RDONLY);
	
	struct Product requestedProduct;
	int requestedPID = atoi(cReq[1]);
	int requiredQty = atoi(cReq[2]);
		
	lseek(fd_prodList, 0, SEEK_SET);
	while (read(fd_prodList, &requestedProduct, sizeof(struct Product)) > 0)
	{
		if (requestedProduct.product_id == requestedPID)
			break;
	}
	
	requestedProduct.quantity = requiredQty;
	
	lseek(fd_cart, 0, SEEK_END);
	write(fd_cart, &requestedProduct, sizeof(struct Product));
	
	close(fd_cart);
	close(fd_prodList);
	
	strcpy(response, "Product added to cart!\n");
}

void editCart(char cReq[][MAX_WORD_SZ], char *response)
{
	int fd_cart = open("ID.cart", O_RDWR, 0777);
	int fd_prodList = open(PRODUCT_FILE, O_CREAT | O_RDONLY, 0777);
	
	struct Product cartItems[MAX_CART_SZ];
	
	int requestedPID = atoi(cReq[1]);
	int requestedQuantity = atoi(cReq[2]);
	
	struct Product requestedProduct;
	
	int items = 0;
	lseek(fd_cart, 0, SEEK_SET);
	while(read(fd_cart, &requestedProduct, sizeof(struct Product)) > 0)
	{
		struct Product cartItem;
		cartItem.product_id = requestedProduct.product_id;
		strcpy(cartItem.product_name, requestedProduct.product_name);
		cartItem.cost = requestedProduct.cost;
		cartItem.quantity = requestedProduct.quantity;
		
		cartItems[items++] = cartItem;
	}
	close(fd_cart);
	
	for (int itemsIter = 0; itemsIter < items; itemsIter++)
	{
		if (cartItems[itemsIter].product_id == requestedPID)
		{
			cartItems[itemsIter].quantity = requestedQuantity;
			break;
		}	
	}
	
	fd_cart = open("ID.cart", O_RDWR | O_TRUNC, 0777);
	lseek(fd_cart, 0, SEEK_SET);
	for (int itemsIter = 0; itemsIter < items; itemsIter++)
	{
		if (cartItems[itemsIter].quantity > 0)
			write(fd_cart, &cartItems[itemsIter], sizeof(struct Product));
	}
	
	close(fd_cart);
	close(fd_prodList);
	
	strcpy(response, "Your Cart is updated!\n");
}

void buyCart(char *request, char *response, int cfd)
{
	int fd_cart = open("ID.cart", O_CREAT | O_RDONLY, 0777);
	int fd_prod = open(PRODUCT_FILE, O_RDWR, 0777);
	
	printf("fd_prod : %d", fd_prod); fflush(stdout);
	perror("open fd_prod");
	
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;
	
	int t = fcntl(fd_prod, F_SETLKW, &lock);
	printf("fcntl = %d\n", t); fflush(stdout);
	perror("Inside critical section");
	
	int totalCost = 0;
	struct Product emptyProd;
	while (read(fd_cart, &emptyProd, sizeof(emptyProd)) > 0)
	{
		// lock here
		totalCost += emptyProd.cost * emptyProd.quantity;
		lseek(fd_prod, 0, SEEK_SET);
		struct Product mainProd;
		while (read(fd_prod, &mainProd, sizeof(mainProd)) > 0)
		{
			if (mainProd.product_id == emptyProd.product_id)
			{
				mainProd.quantity -= emptyProd.quantity;
				lseek(fd_prod, -sizeof(struct Product), SEEK_CUR);
				write(fd_prod, &mainProd, sizeof(struct Product));
				break;
			}
		}
	}
	
	
	lock.l_type = F_UNLCK;
	fcntl(fd_prod, F_SETLK, &lock);
	perror("Outside Critical Section");
		
	
	sprintf(response, "Your total amount payable for the cart is %d. Enter amount in terminal.\n", totalCost);
	write(cfd, response, MAX_MESSAGE_LEN);
	read(cfd, request, MAX_MESSAGE_LEN);
	
	// release lock
	strcpy(response, "You payment was successful.\n");
	
}




