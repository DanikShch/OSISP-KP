#include "socket_wrapper.h"
#include "file_trancfer.h"
#include <unistd.h>


int main() {
    int fd = Socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in adr = {0};
    adr.sin_family = AF_INET;
    adr.sin_port = htons(53535);
    Inet_pton(AF_INET, "127.0.0.1", &adr.sin_addr);
    Connect(fd, &adr, sizeof(adr));
    send_file("file1",fd);
    send_file("file2",fd);
    close(fd);
}