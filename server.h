#ifndef FILESHARING_SERVER_H
#define FILESHARING_SERVER_H

void acceptConnection(const int *server, int *fd[], struct sockaddr_in *adr, int count);
void openServer(int port, int *server, struct sockaddr_in *adr, int count);

#endif //FILESHARING_SERVER_H
