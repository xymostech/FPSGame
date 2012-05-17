#ifndef POLYGON_H
#define POLYGON_H

#include <GL/glfw.h>
#include <stdlib.h>

#include "vector.h"

struct point {
	float x, y, z;

	struct point *next;
};

struct point* point_init(float, float, float);
struct point* point_copy(struct point*);
void point_delete(struct point*);

void point_draw(struct point*);

struct polygon {
	struct point *points;

	struct vector a, u, v, n;
	float dot_uv, dot_uu, dot_vv;
	float inv_denom;

	struct polygon *next;
};

struct polygon* polygon_init();
void polygon_delete(struct polygon*);

void polygon_add_point(struct polygon*, struct point*);
void polygon_setup_hittest(struct polygon*);

void polygon_draw(struct polygon*);

float polygon_hittest(struct polygon*, struct vector, struct vector);

#endif /* POLYGON_H */
