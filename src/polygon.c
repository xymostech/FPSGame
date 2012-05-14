#include "polygon.h"

struct point* point_init(float x, float y, float z) {
	struct point* point = malloc(sizeof(*point));

	point->x = x;
	point->y = y;
	point->z = z;

	return point;
}

void point_delete(struct point *point) {
	free(point);
}

void point_draw(struct point *point) {
	glVertex3f(point->x, point->y, point->z);
}

struct polygon* polygon_init() {
	struct polygon* poly = malloc(sizeof(*poly));

	poly->points = NULL;

	return poly;
}

void polygon_delete(struct polygon *poly) {
	free(poly);
}

void polygon_add_point(struct polygon *poly, struct point *point) {
	point->poly_next = poly->points;
	poly->points = point;
}

void polygon_draw(struct polygon *poly) {
	glBegin(GL_POLYGON);
	struct point *point = poly->points;
	while (point) {
		point_draw(point);
		point = point->poly_next;
	}
	glEnd();
}
