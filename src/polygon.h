#ifndef POLYGON_H
#define POLYGON_H

#include <stdlib.h>

struct point {
	float x, y, z;

	struct point *next;
};

struct point* point_init(float, float, float);
void point_delete(struct point*);

struct polygon {
	struct point *points;

	struct polygon *next;
};

struct polygon* polygon_init();
void polygon_delete(struct polygon*);

void polygon_add_point(struct polygon*, struct point*);

#endif /* POLYGON_H */
