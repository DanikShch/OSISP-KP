CC = gcc
CFLAGS = -W -Wall -Wno-unused-parameter -Wno-unused-variable -std=c11 -pedantic
.PHONY:clean

all: server client

server: server.c file_trancfer.c socket_wrapper.c Makefile
	$(CC) $(CFLAGS) server.c file_trancfer.c socket_wrapper.c -o server

client: client.c file_trancfer.c socket_wrapper.c Makefile
	$(CC) $(CFLAGS) client.c file_trancfer.c socket_wrapper.c -o client

clean:
	rm -f client server