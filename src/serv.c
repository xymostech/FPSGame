/*
 * serv.c
 *
 * entry point for the serv program
 */

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

#include "client.h"
#include "data.h"

#define BUFFER_SIZE 1500

/*
 * make_socket(void)
 *
 * creates a bound socket to read data from
 *
 * returns: the socket, or -1 if error
 */
int make_socket() {
	int sock;

	struct addrinfo hints, *info, *p;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;

	int result;

	if ((result = getaddrinfo(NULL, "1234", &hints, &info)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(result));
		return -1;
	}

	for (p=info; p!=NULL; p = p->ai_next) {
		if ((sock = socket(p->ai_family,
		                   p->ai_socktype,
		                   p->ai_protocol)) == -1) {
			perror("socket");
			continue;
		}

		if (bind(sock, p->ai_addr, p->ai_addrlen) == -1) {
			close(sock);
			perror("bind");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "could not bind socket\n");
		return -1;
	}

	freeaddrinfo(info);

	return sock;
}

void handle_new_connection(struct client *clients, char *buffer, int sock, struct client *client) {
	printf("New connection: %d\n", client->id);
	packet_send_id(sock, client);
	struct client *loop = clients->next;
	while (loop != clients) {
		if (loop->id != client->id) {
			packet_send_connect(sock, loop, client);
			packet_send_connect(sock, client, loop);
		}
		loop = loop->next;
	}
}

void handle_disconnect(struct client *clients, char *buffer, int sock) {
	int id = data_unpack_int16(buffer+2);
	printf("Client disconnected: %d\n", id);
	struct client *client = client_lookup(clients, id);
	if (client) {
		client->prev->next = client->next;
		client->next->prev = client->prev;
		struct client *loop = clients->next;
		while (loop != clients) {
			packet_send_disconnect(sock, loop, client);
			loop = loop->next;
		}
		client_delete(client);
	}
}

void handle_position_update(struct client *clients, char *buffer, int sock) {
	int id = data_unpack_int16(buffer+2);
	float x, y, z, yvel;
	data_unpack_float32(buffer+4, &x);
	data_unpack_float32(buffer+8, &y);
	data_unpack_float32(buffer+12, &z);
	data_unpack_float32(buffer+16, &yvel);
	struct client *client = client_lookup(clients, id);
	if (client) {
		client->player.x = x;
		client->player.y = y;
		client->player.z = z;
		client->player.yvel = yvel;
		struct client *loop = clients->next;
		while (loop != clients) {
			if (loop->id != id) {
				packet_send_position(sock, loop, client);
			}
			loop = loop->next;
		}
	}
}

void handle_hit_update(struct client *clients, char *buffer, int sock) {
	int id = data_unpack_int16(buffer+2);
	int hit = data_unpack_int16(buffer+4);
	struct client *client = client_lookup(clients, hit);
	if (client) {
		client->player.x = 0;
		client->player.y = 0.1;
		client->player.z = 0;
		packet_send_respawn(sock, client);
	}
}

/*
 * main
 *
 * main entry function for the server program
 */
int main(int argc, char const *argv[])
{
	int sock = make_socket();

	if (sock < 0) {
		return 1;
	}

	struct sockaddr_storage recv_addr;
	int addr_len;

	unsigned char buffer[BUFFER_SIZE];

	struct client *clients = malloc(sizeof(*clients));
	clients->next = clients;
	clients->prev = clients;
	int result, client_max_id = 1;

	while (1) {
		addr_len = sizeof(recv_addr);
		if ((result = recvfrom(sock, buffer, BUFFER_SIZE, 0,
		                       (struct sockaddr*)&recv_addr,
		                       &addr_len)) == -1) {
			perror("recvfrom");
			continue;
		}

		int pos = 0;
		int type = data_unpack_int16(buffer);
		if (type == 1) {
			/* manually add new client into clients */
			struct client *client =
			     client_init(client_max_id,
			                 (struct sockaddr*)&recv_addr,
			                 addr_len);
			client_max_id++;
			client->next = clients->next;
			client->prev = clients;
			clients->next->prev = client;
			clients->next = client;
			handle_new_connection(clients, buffer, sock, client);
		} else if (type == 2) {
			handle_disconnect(clients, buffer, sock);
		} else if (type == 5) {
			handle_position_update(clients, buffer, sock);
		} else if (type == 6) {
			handle_hit_update(clients, buffer, sock);
		} else {
			printf("Got unknown packet type: %d\n", type);
		}
	}

	return 0;
}
