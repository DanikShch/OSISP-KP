#include <stdio.h>
#include <unistd.h>
#include <malloc.h>
#include <netinet/in.h>
#include <pthread.h>
#include "server.h"
#include "client.h"
#include "file_transfer.h"
#define THREADS_COUNT 5

int getFreeThread(const int *threads) {
    for (int i = 0; i < THREADS_COUNT; i++) {
        if(threads[i] == 0)
            return i;
    }
    return -1;
}

typedef struct {
    char *filename;
    int fd;
    int * thread;
} info;

void *sendFile(void *arg) {
    info * args = (info *)arg;
    send_file(args->filename, args->fd);;
    *(args->thread) = 0;
    return NULL;
}

void *recvFile(void *arg) {
    info * args = (info *)arg;
    recieve_file(args->fd, args->filename);
    *(args->thread) = 0;
    return NULL;
}

int main() {
    struct sockaddr_in adr = {0};
    char filePath[BUFFER_SIZE];
    char filename[FILENAME_MAX];
    pthread_t thread[THREADS_COUNT];
    int threads[THREADS_COUNT] = {0};
    int *fd[THREADS_COUNT], server;
    int choice = -1;
    int port = 53535;
    int serverOpened = 0, connected = 0;
    char ip[16];
    for (int i = 0; i < THREADS_COUNT; i++) {
        fd[i] = (int*)malloc(sizeof(int));
    }
    do {
        printf("\n=== Меню ===\n");
        printf("1. Запустить сервер\n");
        printf("2. Подключиться к серверу\n");
        printf("3. Принять подключение\n");
        printf("4. Отправить файл\n");
        printf("5. Принять файл\n");
        printf("6. Закрыть соединение\n");
        printf("7. Закрыть сервер\n");
        printf("8. Выход\n\n");
        printf("Введите номер соответствующей функции, которую вы хотите выполнить:\n");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                if(!serverOpened) {
                    openServer(port, &server, &adr, THREADS_COUNT);
                    serverOpened = 1;
                    print_local_ip(port);
                } else {
                    printf("Ошибка: Сервер уже запущен\n");
                    print_local_ip(port);
                }
                break;
            case 2:
                if(!connected) {
                    printf("Введите IP сервера\n");
                    scanf("%s",ip);
                    connectToServer(ip, port, fd, THREADS_COUNT);
                    connected = 1;
                    printf("Соединение с сервером установлено\n");
                } else {
                    printf("Ошибка: Соединение с сервером уже установлено\n");
                }
                break;
            case 3:
                if(serverOpened) {
                    if(!connected) {
                        acceptConnection(&server, fd, &adr, THREADS_COUNT);
                        connected = 1;
                        printf("Принято подключение клиента\n");
                    } else {
                        printf("Ошибка: Подключение клиента уже принято\n");
                    }
                } else {
                    printf("Ошибка: Сервер не запущен\n");
                }
                break;
            case 4:
                if(connected) {
                    inputFileName(filename);
                    int index = -1;
                    while(index == -1) {
                        index = getFreeThread(threads);
                    }
                    threads[index] = 1;
                    info args;
                    args.fd = (*fd)[index];
                    args.filename = filename;
                    args.thread = &(threads[index]);
                    pthread_create(&(thread[index]), NULL, sendFile, &args);
                } else {
                    printf("Ошибка: Соединение не установлено\n");
                }
                break;
            case 5:
                if(connected) {
                    int index = -1;
                    while(index == -1) {
                        index = getFreeThread(threads);
                    }
                    threads[index] = 1;
                    inputPath(filePath);
                    info args;
                    args.fd = (*fd)[index];
                    args.filename = filePath;
                    args.thread = &(threads[index]);
                    pthread_create(&(thread[index]), NULL, recvFile, &args);
                } else {
                    printf("Ошибка: Соединение не установлено\n");
                }
                break;
            case 6:
                if (connected) {
                    for (int i = 0; i < THREADS_COUNT; i++) {
                        close((*fd)[i]);
                    }
                    connected = 0;
                }
                printf("Соединение закрыто\n");
                break;

            case 7:
                if(connected) {
                    for (int i = 0; i < THREADS_COUNT; ++i) {
                        close((*fd)[i]);
                    }
                }
                if(serverOpened) {
                    close(server);
                }
                connected = 0;
                serverOpened = 0;
                printf("Сервер выключен\n");
                break;
            case 8:
                for (int i = 0; i < THREADS_COUNT; ++i) {
                    close((*fd)[i]);
                }
                close(server);
                connected = 0;
                serverOpened = 0;
                printf("Выход...\n");
                break;
            default:
                printf("Неверный выбор. Пожалуйста, выберите существующую опцию.\n");
                break;
        }
        sleep(1);
    } while (choice != 8);
    for (int i = 0; i < THREADS_COUNT; i++) {
        free(fd[i]);
    }
    return 0;
}