#include "protocol.h"

void userListProducts(char *response);
void userListCart(char *response);
void addToCart(char [][MAX_WORD_SZ], char *response);
void editCart(char [][MAX_WORD_SZ], char *response);
void buyCart(char *request, char *response, int cfd);
