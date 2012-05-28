#ifndef CLIENT_H
#define CLIENT_H

#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "servplayer.h"

struct client {
	int id;

	struct sockaddr_storage addr;
	size_t addrlen;

	struct servplayer player;

	struct client *next, *prev;
};

struct client* client_init(int, struct sockaddr*, size_t);
void client_delete(struct client*);

int client_sendpacket(struct client*, int, unsigned char*, int);

#endif /* CLIENT_H */
