#include "socket_wrapper.h"
#include "file_transfer.h"
#include <unistd.h>
#include <stdio.h>

void connectToServer(char *ip, int port, int * fd[], int count) {
    int success = 0;
    do {
        printf("Введите IP сервера\n");
        scanf("%s",ip);
        for (int i = 0; i < count; i++) {
            (*fd)[i] = Socket(AF_INET, SOCK_STREAM, 0);
            struct sockaddr_in adr = {0};
            adr.sin_family = AF_INET;
            adr.sin_port = htons(port);
            if (Inet_pton(AF_INET, ip, &adr.sin_addr) <= 0) {
                printf("Некорректный IP-адрес. Повторите ввод.\n");
                success = 0;
                break;
            }
            if (Connect((*fd)[i], &adr, sizeof(adr)) < 0) {
                printf("Не удалось подключиться к серверу. Повторите ввод.\n");
                success = 0;
                break;
            }
            success = 1;
        }
    } while (!success);

}
