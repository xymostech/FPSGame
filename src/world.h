#ifndef WORLD_H
#define WORLD_H

struct world_object {
	int type;
	struct world_object *next;
};

void world_object_delete(struct world_object*);

struct world_floor {
	int type;
	struct world_object *next;

	float x1, y1, x2, y2;
};

struct world_object* world_floor_init(float, float, float, float);

struct world {
	struct world_object *objects;
};

struct world* world_init();
void world_delete(struct world*);

#endif /* WORLD_H */
