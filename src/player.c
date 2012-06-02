#include "player.h"
#include "world.h"
#include "server.h"

struct player* player_init(int id) {
	struct player *player = malloc(sizeof(*player));

	player->id = id;

	player->x = 0;
	player->y = 0.001;
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

void player_selfupdate(struct player *player, struct world *world, struct server *server) {
	float xvel=0, yvel=0, dist;
	if (glfwGetKey('W')) {
		xvel += 0.05*cos(player->xang);
		yvel += 0.05*sin(player->xang);
	} else if (glfwGetKey('S')) {
		xvel -= 0.05*cos(player->xang);
		yvel -= 0.05*sin(player->xang);
	}
	if (glfwGetKey('A')) {
		xvel += 0.05*sin(player->xang);
		yvel -= 0.05*cos(player->xang);
	} else if (glfwGetKey('D')) {
		xvel -= 0.05*sin(player->xang);
		yvel += 0.05*cos(player->xang);
	}

	dist = sqrt(xvel*xvel + yvel*yvel);
	if (dist > 0.01) {
		player->x += xvel / dist * 0.05;
		player->z += yvel / dist * 0.05;
	}

	int x, y;

	mouse_get_mouselook_diff(&x, &y);

	player->xang += x / 200.0;
	player->yang -= y / 200.0;

	if (player->yang < 0.01) {
		player->yang = 0.01;
	}
	if (player->yang > M_PI-0.01) {
		player->yang = M_PI-0.01;
	}

	if (player->on_ground && key_just_pressed(' ')) {
		player->yvel += 0.1;
	}

	server_position_update(server, player);

	struct vector start = vect(player->x, player->y + 0.4, player->z);
	struct vector dir = vect(cos(player->xang)*sin(player->yang),
	                         cos(player->yang),
	                         sin(player->xang)*sin(player->yang));
	
	struct world_object *hit_object = NULL, *object = world->objects->next;
	float best_dist = -1, test_dist;
	while (object != world->objects) {
		if (object->does_hit) {
			test_dist = world_object_hittest(object, start, dir);
			if (best_dist < 0 && test_dist > 0) {
				best_dist = test_dist;
				hit_object = object;
			} else if (test_dist > 0 && test_dist < best_dist) {
				best_dist = test_dist;
				hit_object = object;
			}
		}
		object = object->next;
	}
	if (hit_object != NULL) {
		world_object_dohit(hit_object);
	}
}

void player_update(struct player *player, struct world *world) {
	float old_y = player->y;
	player->yvel -= 0.005;
	player->y += player->yvel;

	int colliding = 0;
	struct world_object *collide = NULL, *object = world->objects->next;
	while (object != world->objects) {
		if (object->type == WORLD_FLOOR) {
			struct world_floor *floor = (struct world_floor*)object;
			if (old_y > floor->y && player->y < floor->y &&
			    player->x > floor->x1 &&
			    player->x < floor->x2 &&
			    player->z > floor->z1 &&
			    player->z < floor->z2) {
				colliding = 1;
				collide = object;
				break;
			}
		}
		object = object->next;
	}

	if (colliding) {
		if (collide->type == WORLD_FLOOR) {
			struct world_floor *floor = (struct world_floor*)collide;
			player->yvel = 0;
			player->on_ground = 1;
			player->y = 0.001 + floor->y;
		}
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
