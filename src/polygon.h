#ifndef POLYGON_H
#define POLYGON_H

#include <GL/glfw.h>
#include <stdlib.h>

struct point {
	float x, y, z;

	struct point *next;
	struct point *poly_next;
};

struct point* point_init(float, float, float);
void point_delete(struct point*);

void point_draw(struct point*);

struct polygon {
	struct point *points;

	struct polygon *next;
};

struct polygon* polygon_init();
void polygon_delete(struct polygon*);

void polygon_add_point(struct polygon*, struct point*);

void polygon_draw(struct polygon*);

#endif /* POLYGON_H */
