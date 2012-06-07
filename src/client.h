/*
 * client.h
 *
 * server-side storage of connected players' data
 */
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
	/* global client id */
	int id;

	/* storage of their address, to send data to */
	struct sockaddr_storage addr;
	size_t addrlen;

	/* storage of their player properties */
	struct servplayer player;

	/* linked list pointers */
	struct client *next, *prev;
};

/* basic init/delete functions */
struct client* client_init(int, struct sockaddr*, size_t);
void client_delete(struct client*);

/* send a packet to a client */
int client_sendpacket(struct client*, int, unsigned char*, int);

/* lookup a client by its id in a linked list */
struct client* client_lookup(struct client*, int);

#endif /* CLIENT_H */
