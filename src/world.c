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

float world_object_hittest(struct world_object *object, struct vector start, struct vector dir) {
	switch (object->type) {
		case WORLD_MODEL:
			return world_model_hittest((struct world_model*)object, start, dir);
		default:
			break;
	}
	return -1;
}

void world_object_dohit(struct world_object *object) {
	switch (object->type) {
		case WORLD_MODEL:
			world_model_dohit((struct world_model*)object);
			break;
		default:
			break;
	}
}

struct world_object* world_floor_init(float x1, float y1, float x2, float y2) {
	struct world_floor *floor = malloc(sizeof(*floor));

	floor->obj.type = WORLD_FLOOR;
	floor->obj.does_hit = 0;

	floor->x1 = x1;
	floor->y1 = y1;
	floor->x2 = x2;
	floor->y2 = y2;

	floor->texture = texture_load("res/debugfloor.png");

	return (struct world_object*)floor;
}

void world_floor_delete(struct world_floor *floor) {
	texture_delete(floor->texture);
	free(floor);
}

void world_floor_draw(struct world_floor *floor) {
	glEnable(GL_TEXTURE_2D);
	texture_use(floor->texture);
	glColor3f(1, 1, 1);
	
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0, 0);
		glVertex3f(floor->x1, 0, floor->y1);
		glTexCoord2f(0, 1);
		glVertex3f(floor->x1, 0, floor->y2);
		glTexCoord2f(1, 0);
		glVertex3f(floor->x2, 0, floor->y1);
		glTexCoord2f(1, 1);
		glVertex3f(floor->x2, 0, floor->y2);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

struct world_object* world_model_init(float x, float y, float z, char *file) {
	struct world_model *model = malloc(sizeof(*model));

	model->obj.type = WORLD_MODEL;
	model->obj.does_hit = 1;

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

	if (model->was_hit) {
		glColor3f(1, 0, 0);
	} else {
		glColor3f(1, 1, 1);
	}

	model->was_hit = 0;

	model_draw(model->model);

	glPopMatrix();
}

float world_model_hittest(struct world_model *model, struct vector start, struct vector dir) {
	struct vector new_start = vector_sub(start, vect(model->x, model->y, model->z));
	return model_hittest(model->model, new_start, dir);
}

void world_model_dohit(struct world_model *model) {
	model->was_hit = 1;
}

struct world* world_init() {
	struct world *world = malloc(sizeof(*world));

	world->objects = NULL;

	world_add_object(world, world_floor_init(-5, -5, 5, 5));
	world_add_object(world, world_model_init(0, 0.5, 0, "res/cube.obj"));
	world_add_object(world, world_model_init(2, 0.5, 0, "res/cube.obj"));
	world_add_object(world, world_model_init(0, 0.5, 2, "res/cube.obj"));

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
