#include "socket_wrapper.h"
#include "file_transfer.h"
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 100
void acceptConnection(const int *server, int *fd[], struct sockaddr_in *adr, int count) {
    for (int i = 0; i < count; i++) {
        (*fd)[i] = Accept(*server, adr, sizeof(*adr));
    }
}

void openServer(int port, int *server, struct sockaddr_in *adr, int count) {
    *server = Socket(AF_INET, SOCK_STREAM, 0);
    adr->sin_family = AF_INET;
    adr->sin_port = htons(port);
    Bind(*server, adr, sizeof(*adr));
    Listen(*server, count);
    printf("Ожидание подключения\n");
}



