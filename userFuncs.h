#include "protocol.h"

void userListProducts(char *response);
void userListCart(char *response, int userID);
void addToCart(char [][MAX_WORD_SZ], char *response, int userID);
void editCart(char [][MAX_WORD_SZ], char *response, int userID);
void buyCart(char *request, char *response, int cfd, int userID);
