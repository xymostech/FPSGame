#include "polygon.h"

struct point* point_init(float x, float y, float z,
                         float tu, float tv,
                         float nx, float ny, float nz) {
	struct point* point = malloc(sizeof(*point));

	point->x = x;
	point->y = y;
	point->z = z;

	point->tu = tu;
	point->tv = tv;

	point->nx = nx;
	point->ny = ny;
	point->nz = nz;

	return point;
}

struct point* point_copy(struct point *point) {
	return point_init(point->x, point->y, point->z,
	                  point->tu, point->tv,
	                  point->nx, point->ny, point->nz);
}

void point_delete(struct point *point) {
	free(point);
}

void point_draw(struct point *point) {
	glTexCoord2f(point->tu, point->tv);
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
	point->next = poly->points;
	poly->points = point;
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
	glBegin(GL_POLYGON);
	struct point *point = poly->points;
	while (point) {
		point_draw(point);
		point = point->next;
	}
	glEnd();
}

float polygon_hittest(struct polygon *poly, struct vector from, struct vector dir) {
	struct vector s = vector_sub(from, poly->a);
	float t = -vector_dot(poly->n, s)/vector_dot(poly->n, dir);

	if (t < 0) {
		return -1;
	}

	struct vector p = vector_add(vector_mult(dir, t), s);

	float dot_up = vector_dot(p, poly->u), dot_vp = vector_dot(p, poly->v);

	float u_d = (poly->dot_vv * dot_up -
	             poly->dot_uv * dot_vp) * poly->inv_denom;
	float v_d = (poly->dot_uu * dot_vp -
	             poly->dot_uv * dot_up) * poly->inv_denom;

	if (u_d > 0 && v_d > 0 && u_d + v_d < 1) {
		return vector_len(dir) * t;
	} else {
		return -1;
	}
}
