#include "player.h"

struct player* player_init() {
	struct player *player = malloc(sizeof(*player));

	player->x = 0;
	player->y = 0;
	player->z = 0;

	player->yang = M_PI_2;
	player->xang = -M_PI_2;

	player->yvel = 0;

	player->cam.x = 0;
	player->cam.y = 0;
	player->cam.z = 0;

	player->cam.yang = M_PI_2;
	player->cam.xang = -M_PI_2;

	return player;
}

void player_update(struct player *player, struct world *world) {
	if (glfwGetKey('W')) {
		player->x += 0.05*cos(player->xang);
		player->z += 0.05*sin(player->xang);
	} else if (glfwGetKey('S')) {
		player->x -= 0.05*cos(player->xang);
		player->z -= 0.05*sin(player->xang);
	}
	if (glfwGetKey('A')) {
		player->xang -= 0.03;
	} else if (glfwGetKey('D')) {
		player->xang += 0.03;
	}

	if (player->on_ground && key_just_pressed(' ')) {
		player->yvel += 0.3;
	}
	player->y += player->yvel;
	player->yvel -= 0.04;
	if (player->y < 0) {
		player->yvel = 0;
		player->on_ground = 1;
	} else {
		player->on_ground = 0;
	}

	player->cam.x = player->x;
	player->cam.y = player->y + 0.4;
	player->cam.z = player->z;
	
	player->cam.xang = player->xang;
	player->cam.yang = player->yang;

	setup_cam(&player->cam);
}

void player_delete(struct player *player) {
	free(player);
}
