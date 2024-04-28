CC = gcc
CFLAGS = -W -Wall -Wno-unused-parameter -Wno-unused-variable -std=c11 -pedantic
.PHONY:clean

all: filesharing

filesharing: main.c client.c server.c file_transfer.c socket_wrapper.c Makefile
	$(CC) $(CFLAGS) main.c client.c server.c file_transfer.c socket_wrapper.c -o filesharing

clean:
	rm -f filesharing