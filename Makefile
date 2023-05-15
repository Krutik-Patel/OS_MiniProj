# compiler
CC=gcc

# target
TARGET_SERVER=server
TARGET_CLIENT=client


all:
	$(CC) -o $(TARGET_SERVER) -pthread server.c requestHandler.c userFuncs.c adminFuncs.c
	$(CC) -o $(TARGET_CLIENT) client.c
	
clean:
	rm $(TARGET_SERVER) $(TARGET_CLIENT)
