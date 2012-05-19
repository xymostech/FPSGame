#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

struct server {
	int socket;

	int id;

	struct sockaddr_storage addr;
	size_t addrlen;
};

struct server* server_init();
void server_delete(struct server*);

void server_connect(struct server*);

#endif /* SERVER_H */
