#include "client.h"

struct client* client_init(int id, struct sockaddr *addr, size_t addrlen) {
	struct client *client = malloc(sizeof(*client));

	client->id = id;
	memcpy(&client->addr, addr, addrlen);
	client->addrlen = addrlen;

	return client;
}

void client_delete(struct client *client) {
	free(client);
}

int client_sendpacket(struct client *client, int sock, unsigned char *packet, int size) {
	return sendto(sock, packet, size, 0, (struct sockaddr*)&client->addr, client->addrlen);
}
