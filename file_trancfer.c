#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>

#define BUFFER_SIZE 1024

void send_file(const char *path, int fd) {
    FILE *file = fopen(path, "rb");
    printf("file %s is open\n", path);
    char buffer[BUFFER_SIZE];
    while (1) {
        sleep(0);
        size_t n = fread(buffer, 1, BUFFER_SIZE, file);
        send(fd,buffer,n,0);
        if(n < BUFFER_SIZE) break;
    }
    fclose(file);
    while (read(fd,buffer,1) < 0);
}

void recieve_file(const char *path, int fd) {
    FILE *file = fopen(path, "wb");
    printf("file %s is open\n", path);
    char buffer[BUFFER_SIZE];
    while (1) {
        size_t n = recv(fd, buffer, BUFFER_SIZE, 0);
        fwrite(buffer,1,n,file);
        if(n < BUFFER_SIZE) break;
    }
    fclose(file);
    write(fd,"#",1);
}