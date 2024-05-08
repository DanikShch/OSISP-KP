#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <libgen.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

// Функция для создания каталогов по указанному пути
int createDirectories(const char* path) {
    char* pathCopy = malloc(strlen(path) + 1);  // Выделение памяти для копии строки
    strcpy(pathCopy, path);  // Копирование строки path в pathCopy
    char* delimiter = strchr(pathCopy, '/');
    while (delimiter != NULL) {
        *delimiter = '\0';  // Разделитель заменяется нулевым символом

        if (mkdir(pathCopy, S_IRWXU | S_IRWXG | S_IRWXO) != 0) {
            free(pathCopy);
            return 0;  // Ошибка при создании каталога
        }

        *delimiter = '/';  // Восстанавливаем разделитель
        delimiter = strchr(delimiter + 1, '/');
    }

    free(pathCopy);
    return 1;  // Все каталоги успешно созданы
}

void inputPath(char * path) {
    printf("Enter file name: ");
    getchar();
    fgets(path, 100, stdin);
    // Удаление символа новой строки в конце имени файла
    size_t length = strlen(path);
    if (length > 0 && path[length - 1] == '\n') {
        path[length - 1] = '\0';
    }
    createDirectories(path);
}

void inputFileName(char* fileName) {
    while (1) {
        printf("Enter file name: ");
        getchar();
        fgets(fileName, 100, stdin);
        // Удаление символа новой строки в конце имени файла
        size_t length = strlen(fileName);
        if (length > 0 && fileName[length - 1] == '\n') {
            fileName[length - 1] = '\0';
        }
        if (access(fileName,R_OK) != -1) {
            break;
        } else {
            printf("File '%s' does not exist or cant read. Please enter a valid file name.\n", fileName);
        }
    }
}

void sendMessage(char *message, int fd) {
    char buffer[BUFFER_SIZE];
    send(fd, message, strlen(message),0);
    recv(fd, buffer, BUFFER_SIZE,0);
}

char* recieveMessage(int fd) {
    rewind(stdin);
    char buffer[BUFFER_SIZE];
    char *answer = "message sent";
    ssize_t n = recv(fd, buffer, BUFFER_SIZE,0);
    send(fd, answer, strlen(answer),0);
    char *message = (char*)malloc((n+1)* sizeof(char));
    strcpy(message, buffer);
    message[n] = '\0';
    return message;
}

char* getFileName(const char* path) {
    char* fileName = basename((char *)path);
    return fileName;
}

long long getFileSize(const char *path) {
    struct stat file_info;
    if (stat(path, &file_info) == 0) {
        long long fileSize = (long long)file_info.st_size;
        return fileSize;
    } else {
        perror("Failed to get file information");
        return -1;
    }
}

// Функция для получения размера файла в виде строки
char* getFileSizeString(long long fileSize) {
    char* fileSizeString = (char*)malloc(20 * sizeof(char));  // Максимальная длина - 20 символов
    sprintf(fileSizeString, "%lld", fileSize);
    return fileSizeString;
}

long long getFileSizeFromString(const char* fileSizeString) {
    char* endPtr;
    long long fileSize = strtoll(fileSizeString, &endPtr, 10);

    if (fileSize == 0 && endPtr == fileSizeString) {
        fprintf(stderr, "Invalid file size string: %s\n", fileSizeString);
        return -1;
    }

    return fileSize;
}

void send_file(const char *path, int fd) {
    char * filename = getFileName(path);
    long long fileSize = getFileSize(path);
    char * fileSizeStr = getFileSizeString(fileSize);
    sendMessage(filename,fd);
    sendMessage(fileSizeStr,fd);
    FILE *file = fopen(path, "rb");
    char buffer[BUFFER_SIZE];
    long long total = 0;
    while (1) {
        sleep(0);
        size_t n = fread(buffer, 1, BUFFER_SIZE, file);
        send(fd,buffer,n,0);
        total+=n;
        if(total == fileSize) break;
    }
    fclose(file);
    printf("file %s has been loaded\n", path);
    recv(fd,buffer,1,0) ;
    printf("file %s has been downloaded\n", path);
    free(fileSizeStr);
}

void recieve_file(int fd, char *filePath) {
    char * filename = recieveMessage(fd);
    char * fileSizeStr = recieveMessage(fd);
    printf("File name: %s\n",filename);
    printf("File size: %s\n",fileSizeStr);
    long long fileSize = getFileSizeFromString(fileSizeStr);
    FILE *file = NULL;
    while (file == NULL) {
        file = fopen(filePath, "wb");
        if (file == NULL) {
            printf("Ошибка при открытии файла: %s\n", filePath);
            inputPath(filePath);
        }
    }
    char buffer[BUFFER_SIZE];
    long long total = 0;
    while (1) {
        size_t n = recv(fd, buffer, BUFFER_SIZE, 0);
        total += n;
        fwrite(buffer,1,n,file);
        if(total >= fileSize) break;
    }
    fclose(file);
    printf("file %s has been downloaded\n", filePath);
    send(fd, "#", 1,0);
    free(filename);
    free(fileSizeStr);
}