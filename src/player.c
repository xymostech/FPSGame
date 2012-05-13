#include "player.h"

struct player* player_init() {
	struct player *player = malloc(sizeof(*player));

	player->x = 0;
	player->y = 0;
	player->z = 0;

	player->cam.x = 0;
	player->cam.y = 0;
	player->cam.z = 0;

	player->cam.yang = M_PI_2;
	player->cam.xang = -M_PI_2;

	return player;
}

void player_update(struct player *player) {
	if (glfwGetKey('W')) {
		player->x += 0.05;
	}

	player->cam.x = player->x;
	player->cam.y = player->y;
	player->cam.z = player->z;

	setup_cam(&player->cam);
}

void player_delete(struct player *player) {
	free(player);
}
