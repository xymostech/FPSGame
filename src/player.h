#ifndef PLAYER_H
#define PLAYER_H

#include <stdlib.h>
#include <math.h>

#include "cam.h"

struct player {
	float x, y, z;
	float xang, yang;
	struct cam cam;
};

struct player* player_init();
void player_update(struct player*);
void player_delete(struct player*);

#endif /* PLAYER_H */
