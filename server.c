#include "socket_wrapper.h"
#include <netinet/in.h>
#include <stdio.h>
#include <ifaddrs.h>
#include <string.h>
#include <arpa/inet.h>

void acceptConnection(const int *server, int *fd[], struct sockaddr_in *adr, int count) {
    printf("Ожидание подключения\n");
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
}

void print_local_ip(int port) {
    struct ifaddrs *addrs, *tmp;
    getifaddrs(&addrs);
    tmp = addrs;

    while (tmp) {
        if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_INET) {
            struct sockaddr_in *pAddr = (struct sockaddr_in *) tmp->ifa_addr;
            char *ip = inet_ntoa(pAddr->sin_addr);

            // Проверяем, что IP-адрес принадлежит локальной сети
            if (strncmp(ip, "192.168.", 8) == 0 || strncmp(ip, "10.", 3) == 0) {
                printf("Server address: %s:%d\n", ip, port);
                break; // Мы нашли IP-адрес в локальной сети, поэтому выходим из цикла
            }
        }
        tmp = tmp->ifa_next;
    }
    freeifaddrs(addrs);
}



