#include "world.h"

void world_object_delete(struct world_object *object) {
	free(object);
}

struct world_object* world_floor_init(float x1, float y1, float x2, float y2) {
	struct world_floor *floor = malloc(sizeof(*floor));

	floor->type = WORLD_FLOOR;

	floor->x1 = x1;
	floor->y1 = y1;
	floor->x2 = x2;
	floor->y2 = y2;

	return (struct world_object*)floor;
}

struct world* world_init() {
	struct world *world = malloc(sizeof(*world));

	world->objects = NULL;

	world_add_object(world, world_floor_init(-5, -5, 5, 5));

	return world;
}

void world_delete(struct world *world) {
	struct world_object *object = world->objects, *next;
	while (object) {
		next = object->next;
		world_object_delete(object);
		object = next;
	}
	free(world);
}

void world_add_object(struct world *world, struct world_object *object) {
	object->next = world->objects;
	world->objects = object;
}
