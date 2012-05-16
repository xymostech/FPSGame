#include "vector.h"

struct vector vect(float x, float y, float z) {
	struct vector v = {x, y, z};
	return v;
}

struct vector vector_add(struct vector a, struct vector b) {
	return vect(a.x + b.x, a.y + b.y, a.z + b.z);
}

struct vector vector_sub(struct vector a, struct vector b) {
	return vect(a.x - b.x, a.y - b.y, a.z - b.z);
}

struct vector vector_mult(struct vector a, float scale) {
	return vect(a.x * scale, a.y * scale, a.z * scale);
}

struct vector vector_div(struct vector a, float scale) {
	return vect(a.x / scale, a.y / scale, a.z / scale);
}

float vector_len(struct vector a) {
	return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

float vector_dot(struct vector a, struct vector b) {
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

struct vector vector_cross(struct vector a, struct vector b) {
	return vect(a.y * b.z - a.z * b.y,
	            a.z * b.x - a.x * b.z,
	            a.x * b.y - a.y * b.x);
}
