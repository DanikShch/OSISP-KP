#include "socket_wrapper.h"
#include "file_transfer.h"
#include <unistd.h>

void connectToServer(char *ip, int port, int * fd[], int count) {

    for (int i = 0; i < count; i++) {
        (*fd)[i] = Socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in adr = {0};
        adr.sin_family = AF_INET;
        adr.sin_port = htons(port);
        Inet_pton(AF_INET, ip, &adr.sin_addr);
        Connect((*fd)[i], &adr, sizeof(adr));
    }
}
