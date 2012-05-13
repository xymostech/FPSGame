#include "player.h"
#include "world.h"

struct player* player_init() {
	struct player *player = malloc(sizeof(*player));

	player->x = 0;
	player->y = 0.01;
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

void player_delete(struct player *player) {
	free(player);
}

void player_selfupdate(struct player *player) {
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
}

void player_update(struct player *player, struct world *world) {
	float old_y = player->y;
	player->yvel -= 0.04;
	player->y += player->yvel;

	int colliding = 0;
	struct world_object *collide = NULL, *object = world->objects;
	while (object) {
		if (object->type == WORLD_FLOOR) {
			struct world_floor *floor = (struct world_floor*)object;
			if (old_y > 0 && player->y < 0 &&
			    player->x > floor->x1 &&
			    player->x < floor->x2 &&
			    player->z > floor->y1 &&
			    player->z < floor->y2) {
				colliding = 1;
				collide = object;
				break;
			}
		}
		object = object->next;
	}

	if (colliding) {
		player->yvel = 0;
		player->on_ground = 1;
		player->y = 0.01;
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
