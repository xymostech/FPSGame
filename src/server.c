#include "server.h"

struct server* server_init() {
	struct server *server = malloc(sizeof(*server));

	return server;
}

void server_delete(struct server *server) {
	free(server);
}

void server_connect(struct server *server) {
	struct addrinfo hints, *info, *p;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;

	int status;

	if ((status = getaddrinfo("127.0.0.1", "1234", &hints, &info)) != 0) {
		fprintf(stderr, "Error with getaddrinfo: %s\n", gai_strerror(status));
		return;
	}

	for (p=info; p!=NULL; p=p->ai_next) {
		if ((server->socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("socket");
			continue;
		}
		break;
	}

	if (p == NULL) {
		fprintf(stderr, "Error making socket\n");
		return;
	}
}
