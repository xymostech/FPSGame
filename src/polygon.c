#include "polygon.h"

struct point* point_init(float x, float y, float z) {
	struct point* point = malloc(sizeof(*point));

	point->x = x;
	point->y = y;
	point->z = z;

	return point;
}

struct point* point_copy(struct point *point) {
	return point_init(point->x, point->y, point->z);
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
	struct point *point = poly->points, *next;
	while (point) {
		next = point->next;
		point_delete(point);
		point = next;
	}
	free(poly);
}

void polygon_add_point(struct polygon *poly, struct point *point) {
	struct point *copy = point_copy(point);
	copy->next = poly->points;
	poly->points = copy;
}

void polygon_setup_hittest(struct polygon *poly) {
	struct vector a, b, c;
	struct point *pt = poly->points;
	a = vect(pt->x, pt->y, pt->z);
	pt = pt->next;
	b = vect(pt->x, pt->y, pt->z);
	pt = pt->next;
	c = vect(pt->x, pt->y, pt->z);

	poly->a = a;
	poly->u = vector_sub(b, a);
	poly->v = vector_sub(c, a);
	poly->n = vector_cross(poly->u, poly->v);

	poly->dot_uv = vector_dot(poly->u, poly->v);
	poly->dot_uu = vector_dot(poly->u, poly->u);
	poly->dot_vv = vector_dot(poly->v, poly->v);

	poly->inv_denom = 1.0 / (poly->dot_uu * poly->dot_vv -
	                         poly->dot_uv * poly->dot_uv);
}

void polygon_draw(struct polygon *poly) {
	glColor3f(1, 1, 1);
	glBegin(GL_POLYGON);
	struct point *point = poly->points;
	while (point) {
		point_draw(point);
		point = point->next;
	}
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	point = poly->points;
	while (point) {
		point_draw(point);
		point = point->next;
	}
	glEnd();
}
