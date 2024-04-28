#ifndef FILESHARING_FILE_TRANSFER_H
#define FILESHARING_FILE_TRANSFER_H
#include <pthread.h>
#define BUFFER_SIZE 1024

void send_file(const char *path, int fd);
void recieve_file(int fd, char *filePath);
void inputFileName(char* fileName);
void inputPath(char * path);

#endif //FILESHARING_FILE_TRANSFER_H
