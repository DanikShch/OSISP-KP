#ifndef FILESHARING_FILE_TRANCFER_H
#define FILESHARING_FILE_TRANCFER_H

void send_file(const char *path, int fd);
void recieve_file(const char *path, int fd);

#endif //FILESHARING_FILE_TRANCFER_H
