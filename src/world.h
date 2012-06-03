#ifndef WORLD_H
#define WORLD_H

#include <GL/glfw.h>
#include <stdlib.h>

#include "model.h"
#include "vector.h"
#include "texture.h"
#include "player.h"
#include "server.h"

enum world_object_type {
	WORLD_FLOOR,
	WORLD_WALL,
	WORLD_MODEL,
	WORLD_PLAYER
};

struct world_object {
	enum world_object_type type;
	struct world_object *next, *prev;
	int does_hit;
};

void world_object_delete(struct world_object*);
void world_object_draw(struct world_object*);
void world_object_update(struct world_object*, struct server*);
float world_object_hittest(struct world_object*, struct vector, struct vector);
void world_object_dohit(struct world_object*);

struct world_floor {
	struct world_object obj;

	float x1, z1, x2, z2, y;

	struct texture *texture;
};

struct world_object* world_floor_init(float, float, float, float, float);
void world_floor_delete(struct world_floor*);
void world_floor_draw(struct world_floor*);

struct world_wall {
	struct world_object obj;

	float x1, y1, z1, x2, y2, z2;
	int same_x;

	struct texture *texture;
};

struct world_object* world_wall_init(float, float, float, float, float, float, int);
void world_wall_delete(struct world_wall*);
void world_wall_draw(struct world_wall*);

struct world_model {
	struct world_object obj;

	float x, y, z;
	struct model *model;

	int was_hit;
};

struct world_object* world_model_init(float, float, float, char*);
void world_model_delete(struct world_model*);
void world_model_draw(struct world_model*);
float world_model_hittest(struct world_model*, struct vector, struct vector);
void world_model_dohit(struct world_model*);

struct world_player {
	struct world_object obj;

	struct model *model;
	struct player *player;

	int was_hit;
};

struct world_object* world_player_init(int, float, float, float, char*);
void world_player_delete(struct world_player*);
void world_player_draw(struct world_player*);
void world_player_update(struct world_player*, struct server*);
float world_player_hittest(struct world_player*, struct vector, struct vector);
void world_player_dohit(struct world_player*);

struct world {
	struct world_object *objects;
};

struct world* world_init();
void world_delete(struct world*);

void world_add_object(struct world*, struct world_object*);

void world_update(struct world*, struct server*);
void world_draw(struct world*);

#endif /* WORLD_H */
