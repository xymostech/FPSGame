#include "client.h"

/*
 * client_init(id, addr, addrlen)
 *
 * basic setup of a client
 */
struct client* client_init(int id, struct sockaddr *addr, size_t addrlen) {
	struct client *client = malloc(sizeof(*client));

	client->id = id;

	/* memcpy the address to store it */
	memcpy(&client->addr, addr, addrlen);
	client->addrlen = addrlen;

	return client;
}

/*
 * client_delete(client)
 *
 * basic delete of a client
 */
void client_delete(struct client *client) {
	free(client);
}

/*
 * client_sendpacket(client, sock, packet, size)
 *
 * sends a packet of a given size to a client
 */
int client_sendpacket(struct client *client, int sock, unsigned char *packet, int size) {
	return sendto(sock, packet, size, 0, (struct sockaddr*)&client->addr, client->addrlen);
}

/*
 * client_lookup(clients, id)
 *
 * looks up a client by its id in a linked list
 *
 * returns: the client with id==id, or NULL if none found
 */
struct client* client_lookup(struct client *clients, int id) {
	/* use a loop because clients are doubly-linked */
	struct client *loop = clients->next;
	while (loop != clients) {
		if (loop->id == id) {
			return loop;
		}
		loop = loop->next;
	}
	return NULL;
}
