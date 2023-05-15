#include <stdio.h>
#include <fcntl.h>
#include "Product.h"
#include <unistd.h>
#include <string.h>

//int main()
//{

//	int fd = open("ProductFile", O_CREAT | O_RDONLY);
//	printf("%d", fd);
//	struct Product prod, emp;
//	lseek(fd, 0, SEEK_SET);
//	prod.product_id = 3;
//	strcpy(prod.product_name, "krutik");
//	prod.cost = 2;
//	prod.quantity = 5;
//	write(fd, &prod, sizeof(prod));
//	perror("write");
//	lseek(fd, 0, SEEK_SET);
//	int t = read(fd, &emp, sizeof(emp));
//	perror("read");
//	printf("%d %d %s %d\n", t, emp.product_id, emp.product_name, emp.cost);
//	close(fd);
//}

int main()
{
	char newMetr[100] = "Hi this is kruik";
	
	printf("%s", newMetr + 4);
}
