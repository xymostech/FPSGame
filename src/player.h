#ifndef PLAYER_H
#define PLAYER_H

#include <stdlib.h>
#include <math.h>

#include "cam.h"
#include "key.h"

struct world;

struct player {
	float x, y, z;
	float xang, yang;

	float yvel;
	int on_ground;

	struct cam cam;
};

struct player* player_init();
void player_update(struct player*, struct world*);
void player_delete(struct player*);

#endif /* PLAYER_H */
