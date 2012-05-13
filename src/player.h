#ifndef PLAYER_H
#define PLAYER_H

#include <stdlib.h>

#include "cam.h"

struct player {
	float x, y, z;
	struct cam cam;
};

struct player* player_init();
void player_update(struct player*);
void player_delete(struct player*);

#endif /* PLAYER_H */
