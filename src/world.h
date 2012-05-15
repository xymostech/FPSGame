#ifndef WORLD_H
#define WORLD_H

#include <GL/glfw.h>
#include <stdlib.h>

#include "model.h"

enum world_object_type {
	WORLD_FLOOR,
	WORLD_MODEL
};

struct world_object {
	enum world_object_type type;
	struct world_object *next;
};

void world_object_delete(struct world_object*);
void world_object_draw(struct world_object*);

struct world_floor {
	enum world_object_type type;
	struct world_object *next;

	float x1, y1, x2, y2;
};

struct world_object* world_floor_init(float, float, float, float);
void world_floor_draw(struct world_floor*);

struct world_model {
	enum world_object_type type;
	struct world_object *next;

	float x, y, z;
	struct model *model;
};

struct world_object* world_model_init(float, float, float, char*);
void world_model_draw(struct world_model*);

struct world {
	struct world_object *objects;
};

struct world* world_init();
void world_delete(struct world*);

void world_add_object(struct world*, struct world_object*);

void world_draw(struct world*);

#endif /* WORLD_H */
