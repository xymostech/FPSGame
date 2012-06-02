#ifndef PLAYER_H
#define PLAYER_H

#include <stdlib.h>
#include <math.h>

#include "cam.h"
#include "key.h"
#include "vector.h"

#include "server.h"

struct world;

struct player {
	int id;

	float x, y, z;
	float xang, yang;

	float xvel, yvel, zvel;
	int on_ground;

	struct cam cam;
};

struct player* player_init(int);
void player_delete(struct player*);

void player_selfupdate(struct player*, struct world*, struct server*);
void player_update(struct player*, struct world*);

#endif /* PLAYER_H */
