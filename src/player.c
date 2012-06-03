#include "player.h"
#include "world.h"
#include "server.h"

struct collide_event* add_collide_event(struct collide_event *next_event,
                                        struct world_object *object) {
	struct collide_event *event = malloc(sizeof(*event));

	event->object = object;
	event->next = next_event;

	return event;
}

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
	player->xvel = 0;
	player->zvel = 0;
	if (glfwGetKey('W')) {
		player->xvel += cos(player->xang);
		player->zvel += sin(player->xang);
	} else if (glfwGetKey('S')) {
		player->xvel -= cos(player->xang);
		player->zvel -= sin(player->xang);
	}
	if (glfwGetKey('A')) {
		player->xvel += sin(player->xang);
		player->zvel -= cos(player->xang);
	} else if (glfwGetKey('D')) {
		player->xvel -= sin(player->xang);
		player->zvel += cos(player->xang);
	}

	float dist = sqrt(player->xvel*player->xvel + player->zvel*player->zvel);

	if (dist > 0.01) {
		player->xvel /= dist / 0.05;
		player->zvel /= dist / 0.05;
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
	player->yvel -= 0.005;

	float old_x = player->x, old_y = player->y, old_z = player->z;
	player->x += player->xvel;
	player->y += player->yvel;
	player->z += player->zvel;

	struct world_object *object = world->objects->next;
	struct collide_event *events = NULL;
	while (object != world->objects) {
		if (object->type == WORLD_FLOOR) {
			struct world_floor *floor = (struct world_floor*)object;
			if (old_y > floor->y && player->y < floor->y &&
			    old_x > floor->x1 &&
			    old_x < floor->x2 &&
			    old_z > floor->z1 &&
			    old_z < floor->z2) {
				events = add_collide_event(events, object);
			}
		} else if (object->type == WORLD_WALL) {
			struct world_wall *wall = (struct world_wall*)object;
			if (wall->same_x) {
				if (player->y + 0.3 > wall->y1 &&
				    player->y       < wall->y2 &&
				    old_z > wall->z1 &&
				    old_z < wall->z2) {
					if ((player->x > wall->x1 &&
					     old_x     < wall->x1) ||
					    (player->x < wall->x1 &&
					     old_x     > wall->x1)) {
						events = add_collide_event(events, object);
					}
				}
			} else {
				if (player->y + 0.30 > wall->y1 &&
				    player->y + 0.01 < wall->y2 &&
				    old_x > wall->x1 &&
				    old_x < wall->x2) {
					if ((player->z > wall->z1 &&
					     old_z     < wall->z1) ||
					    (player->z < wall->z1 &&
					     old_z     > wall->z1)) {
						events = add_collide_event(events, object);
					}
				}
			}
		}
		object = object->next;
	}

	player->on_ground = 0;

	struct collide_event *next_event;
	while (events) {
		next_event = events->next;
		struct world_object *collide = events->object;
		if (collide->type == WORLD_FLOOR) {
			struct world_floor *floor = (struct world_floor*)collide;
			player->yvel = 0;
			player->on_ground = 1;
			player->y = 0.001 + floor->y;
		} else if (collide->type == WORLD_WALL) {
			struct world_wall *wall = (struct world_wall*)collide;
			if (wall->same_x) {
				player->x = old_x;
			} else {
				player->z = old_z;
			}
		}
		free(events);
		events = next_event;
	}

	player->cam.x = player->x;
	player->cam.y = player->y + 0.4;
	player->cam.z = player->z;
	
	player->cam.xang = player->xang;
	player->cam.yang = player->yang;

	setup_cam(&player->cam);
}
