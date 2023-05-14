#include "protocol.h"

char currRequest[MAX_REQ_WORDS][MAX_WORD_SZ];
void handleClientRequest(char *request, char *response, int accountType);
void parseResponse(char *);


