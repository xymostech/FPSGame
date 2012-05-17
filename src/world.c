#include "world.h"

void world_object_delete(struct world_object *object) {
	switch (object->type) {
		case WORLD_FLOOR:
			world_floor_delete((struct world_floor*)object);
			break;
		case WORLD_MODEL:
			world_model_delete((struct world_model*)object);
			break;
		default:
			break;
	}
}

void world_object_draw(struct world_object *object) {
	switch (object->type) {
		case WORLD_FLOOR:
			world_floor_draw((struct world_floor*)object);
			break;
		case WORLD_MODEL:
			world_model_draw((struct world_model*)object);
			break;
		default:
			break;
	}
}

struct world_object* world_floor_init(float x1, float y1, float x2, float y2) {
	struct world_floor *floor = malloc(sizeof(*floor));

	floor->obj.type = WORLD_FLOOR;

	floor->x1 = x1;
	floor->y1 = y1;
	floor->x2 = x2;
	floor->y2 = y2;

	return (struct world_object*)floor;
}

void world_floor_delete(struct world_floor *floor) {
	free(floor);
}

void world_floor_draw(struct world_floor *floor) {
	glColor3f(1, 1, 1);
	
	glBegin(GL_TRIANGLE_STRIP);
		glVertex3f(floor->x1, 0, floor->y1);
		glVertex3f(floor->x1, 0, floor->y2);
		glVertex3f(floor->x2, 0, floor->y1);
		glVertex3f(floor->x2, 0, floor->y2);
	glEnd();
}

struct world_object* world_model_init(float x, float y, float z, char *file) {
	struct world_model *model = malloc(sizeof(*model));

	model->obj.type = WORLD_MODEL;

	model->x = x;
	model->y = y;
	model->z = z;

	model->model = model_load(file);

	return (struct world_object*)model;
}

void world_model_delete(struct world_model *model) {
	model_delete(model->model);
	free(model);
}

void world_model_draw(struct world_model *model) {
	glPushMatrix();

	glTranslatef(model->x, model->y, model->z);

	model_draw(model->model);

	glPopMatrix();
}

struct world* world_init() {
	struct world *world = malloc(sizeof(*world));

	world->objects = NULL;

	world_add_object(world, world_floor_init(-5, -5, 5, 5));
	world_add_object(world, world_model_init(0, 0.5, 0, "res/cube.obj"));

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

void world_draw(struct world *world) {
	struct world_object *object = world->objects;
	while (object) {
		world_object_draw(object);
		object = object->next;
	}
}
