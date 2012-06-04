#include "server.h"

#include "player.h"

#include "world.h"

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

	fcntl(server->socket, F_SETFL, O_NONBLOCK);

	memcpy(&server->addr, p->ai_addr, p->ai_addrlen);
	server->addrlen = p->ai_addrlen;

	freeaddrinfo(info);

	server_connect_packet(server);

	fd_set master, test;
	struct timeval tv;

	tv.tv_sec = 1;
	tv.tv_usec = 0;

	FD_ZERO(&master);
	FD_SET(server->socket, &master);

	char buffer[1024];
	int len;

	while (1) {
		test = master;
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		if (select(server->socket+1, &test, NULL, NULL, &tv) == -1) {
			perror("select");
			return;
		}
		if (FD_ISSET(server->socket, &test)) {
			len = recvfrom(server->socket, buffer, 1024, 0, NULL, NULL);
			if (len == 4) {
				int type = data_unpack_int16(buffer);
				if (type == 3) {
					server->id = data_unpack_int16(buffer+2);
					break;
				}
			}
		}
		server_connect_packet(server);
	}
}

void server_disconnect(struct server *server) {
	server_disconnect_packet(server);
}

void server_position_update(struct server *server, struct player *player) {
	server_position_packet(server, player);
}

void server_sendpacket(struct server *server, unsigned char *packet, int len) {
	sendto(server->socket, packet, len, 0, (struct sockaddr*)&server->addr, server->addrlen);
}

void server_recvpacket(struct server *server, unsigned char *buffer, int *len) {
	*len = recvfrom(server->socket, buffer, *len, 0, NULL, NULL);
}

void server_connect_packet(struct server *server) {
	unsigned char packet[2];
	data_pack_int16(packet, 1);

	server_sendpacket(server, packet, 2);
}

void server_disconnect_packet(struct server *server) {
	unsigned char packet[4];
	data_pack_int16(packet, 2);
	data_pack_int16(packet+2, server->id);

	server_sendpacket(server, packet, 4);
}

void server_position_packet(struct server *server, struct player *player) {
	unsigned char packet[20];

	data_pack_int16(packet, 5);
	data_pack_int16(packet+2, server->id);
	data_pack_float32(packet+4, player->x);
	data_pack_float32(packet+8, player->y);
	data_pack_float32(packet+12, player->z);
	data_pack_float32(packet+16, player->yvel);

	server_sendpacket(server, packet, 20);
}

void server_hit_packet(struct server *server, struct player *player) {
	unsigned char packet[6];

	data_pack_int16(packet, 6);
	data_pack_int16(packet+2, server->id);
	data_pack_int16(packet+4, player->id);

	server_sendpacket(server, packet, 6);
}

void server_handle_updates(struct server *server, struct player *my_player, struct world *world) {
	char buffer[1024];
	while (1) {
		int len = recvfrom(server->socket, buffer, 1024, 0, NULL, NULL);
		if (len == -1) {
			break;
		}
		int type = data_unpack_int16(buffer);
		if (type == 1) {
			int id = data_unpack_int16(buffer+2);
			world_add_object(world, world_player_init(id, 0.0, 0.0, 0.0, "res/cube.obj"));
		} else if (type == 4) {
			float x, y, z;
			data_unpack_float32(buffer+2, &x);
			data_unpack_float32(buffer+6, &y);
			data_unpack_float32(buffer+10, &z);
			my_player->x = x;
			my_player->y = y;
			my_player->z = z;
		} else if (type == 5) {
			int id = data_unpack_int16(buffer+2);
			float x, y, z, yvel;
			data_unpack_float32(buffer+4, &x);
			data_unpack_float32(buffer+8, &y);
			data_unpack_float32(buffer+12, &z);
			data_unpack_float32(buffer+16, &yvel);
			struct world_object *object = world->objects->next;
			while (object != world->objects) {
				if (object->type == WORLD_PLAYER) {
					struct world_player *player = (struct world_player*)object;
					if (player->player->id == id) {
						player->player->x = x;
						player->player->y = y;
						player->player->z = z;
						player->player->yvel = yvel;
					}
				}
				object = object->next;
			}
		}
	}
}
