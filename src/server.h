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
#include <fcntl.h>

struct world;
struct player;

struct server {
	int socket;

	int id;

	struct sockaddr_storage addr;
	socklen_t addrlen;
};

struct server* server_init();
void server_delete(struct server*);

void server_connect(struct server*);
void server_disconnect(struct server*);
void server_position_update(struct server*, struct player*);

void server_sendpacket(struct server*, unsigned char*, int);
void server_recvpacket(struct server*, unsigned char*, int*);

void server_connect_packet(struct server*);
void server_disconnect_packet(struct server*);
void server_position_packet(struct server*, struct player*);
void server_hit_packet(struct server*, struct player*);

void server_handle_updates(struct server*, struct player*, struct world*);

#endif /* SERVER_H */
